#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/pwm.h"
#include "bsp/board.h"
#include "pico/stdlib.h"
#include "pinout.h"
#include "usb_descriptors.h"
#include "controller_config.h"

uint8_t  g_lamp_state[NUM_LIGHTS] = {0};
uint16_t g_button_state = 0;
uint64_t g_curr_time    = 0;
uint32_t g_hid_last     = 0;

static uint8_t buttonStatus[22] = {0};

const uint g_led_pin[] = {PIN_JOY_RGB_R,PIN_JOY_RGB_G,PIN_JOY_RGB_B,PIN_RFID_RGB_R,PIN_RFID_RGB_G,PIN_RFID_RGB_B,PIN_LB_LED,PIN_RB_LED};
const uint g_but_pin[] = {PIN_LB_UP,PIN_LB_DOWN,PIN_LB_LEFT,PIN_LB_RIGHT,PIN_LB_BUTTON,PIN_RB_UP,PIN_RB_DOWN,PIN_RB_LEFT,PIN_RB_RIGHT,PIN_RB_BUTTON,PIN_TEST,PIN_SERVICE,PIN_COIN};   

typedef struct joy_report_s {
	uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
	uint8_t  HAT;    // HAT switch; one nibble w/ unused nibble
	uint8_t  LX;     // Left  Stick X
	uint8_t  LY;     // Left  Stick Y
	uint8_t  RX;     // Right Stick X
	uint8_t  RY;     // Right Stick Y
	uint8_t  VendorSpec;
} joy_report_t;

static void update_state_joy(uint16_t button_state)
{
	// note: BUTTONCAPTURE is the card scan button 
	static uint8_t order[] = {BUTTONUP,BUTTONDOWN,BUTTONLEFT,BUTTONRIGHT,BUTTONLB,BUTTONX,BUTTONB,BUTTONY,BUTTONA,BUTTONRB,BUTTONSTART,BUTTONSELECT,BUTTONHOME};
	
	/* buttons */
	for (int i=0; i<NUM_BUTTONS; i++)
	{ 
		if ((button_state>>i)&1) buttonStatus[order[i]] = 1; 
		else buttonStatus[order[i]] = 0;
	}
	
	buttonStatus[AXISLX] = 128;
	buttonStatus[AXISLY] = 128;
	buttonStatus[AXISRX] = 128;
	buttonStatus[AXISRY] = 128;
}

void generate_report_joy(joy_report_t *report){  
// HAT
    if ((buttonStatus[BUTTONUP]) && (buttonStatus[BUTTONRIGHT])){report->HAT = DPAD_UPRIGHT_MASK_ON;}
    else if ((buttonStatus[BUTTONDOWN]) && (buttonStatus[BUTTONRIGHT])) {report->HAT = DPAD_DOWNRIGHT_MASK_ON;} 
    else if ((buttonStatus[BUTTONDOWN]) && (buttonStatus[BUTTONLEFT])) {report->HAT = DPAD_DOWNLEFT_MASK_ON;}
    else if ((buttonStatus[BUTTONUP]) && (buttonStatus[BUTTONLEFT])){report->HAT = DPAD_UPLEFT_MASK_ON;}
    else if (buttonStatus[BUTTONUP]) {report->HAT = DPAD_UP_MASK_ON;}
    else if (buttonStatus[BUTTONDOWN]) {report->HAT = DPAD_DOWN_MASK_ON;}
    else if (buttonStatus[BUTTONLEFT]) {report->HAT = DPAD_LEFT_MASK_ON;}
    else if (buttonStatus[BUTTONRIGHT]) {report->HAT = DPAD_RIGHT_MASK_ON;}
    else{report->HAT = DPAD_NOTHING_MASK_ON;}

// analogs
	report->LX = buttonStatus[AXISLX];
	report->LY = buttonStatus[AXISLY];
	report->RX = buttonStatus[AXISRX];
	report->RY = buttonStatus[AXISRY];

// Buttons
	if (buttonStatus[BUTTONA]) {report->Button |= A_MASK_ON;}
	if (buttonStatus[BUTTONB]) {report->Button |= B_MASK_ON;}
	if (buttonStatus[BUTTONX]) {report->Button |= X_MASK_ON;}
	if (buttonStatus[BUTTONY]) {report->Button |= Y_MASK_ON;}
	if (buttonStatus[BUTTONLB]) {report->Button |= LB_MASK_ON;}
	if (buttonStatus[BUTTONRB]) {report->Button |= RB_MASK_ON;}
	if (buttonStatus[BUTTONLT]) {report->Button |= ZL_MASK_ON;}
	if (buttonStatus[BUTTONRT]) {report->Button |= ZR_MASK_ON;}
	if (buttonStatus[BUTTONSTART]){report->Button |= START_MASK_ON;}
	if (buttonStatus[BUTTONSELECT]){report->Button |= SELECT_MASK_ON;}
	if (buttonStatus[BUTTONHOME]){report->Button |= HOME_MASK_ON;}
	if (buttonStatus[BUTTONL3]){report->Button |= L3_MASK_ON;}
	if (buttonStatus[BUTTONR3]){report->Button |= R3_MASK_ON;}
	if (buttonStatus[BUTTONCAPTURE]){report->Button |= CAPTURE_MASK_ON;}
}


/**
 * Update Input States
 **/
void update_inputs() {
	static uint32_t long_press = 0;
	static bool prev_service = 0;
	uint16_t button_state = 0;
	
	for (int i=0; i<NUM_BUTTONS; i++)
	{
			gpio_put(25, 0);   	
		if (!gpio_get(g_but_pin[i]))
		{
			button_state |= 1<<(i);
		}
	}	

	g_button_state = button_state;
}

uint32_t pwm_set_freq_duty(uint slice_num,
       uint chan,uint32_t f, int d)
{
 uint32_t clock = 125000000;
 uint32_t divider16 = clock / f / 4096 + 
                           (clock % (f * 4096) != 0);
 if (divider16 / 16 == 0)
 divider16 = 16;
 uint32_t wrap = clock * 16 / divider16 / f - 1;
 pwm_set_clkdiv_int_frac(slice_num, divider16/16,
                                     divider16 & 0xF);
 pwm_set_wrap(slice_num, wrap);
 pwm_set_chan_level(slice_num, chan, wrap * d / 100);
 return wrap;
}

void update_lights(){
	for (int i=0; i<NUM_LIGHTS; i++)
	{
		uint slice_num = pwm_gpio_to_slice_num(g_led_pin[i]);
		uint chan = pwm_gpio_to_channel(g_led_pin[i]);
		uint duty = (255 - g_lamp_state[i])*100/255;
		pwm_set_freq_duty(slice_num,chan, 60, duty);
		pwm_set_enabled(slice_num, true);
	}	
}

/**
 * Reactive Button Lights
 **/
void update_lights_reactive() {
  if (REACTIVE_FALLBACK(g_hid_last))
  {
	  if ( g_button_state & 0x1EF ) // joysticks
	  {
		  g_lamp_state[0] = 0x7F;
		  g_lamp_state[1] = 0x00;
		  g_lamp_state[2] = 0x23;	
	  }
	  else
	  {
		  g_lamp_state[0] = 0x00;
		  g_lamp_state[1] = 0x7F;
		  g_lamp_state[2] = 0x23; 	  
	  }
	  
	// left button
	  g_lamp_state[6] = ( g_button_state & 0x10 ) ? 0xFF : 0x00;
	
	//right button
	g_lamp_state[7] = ( g_button_state & 0x200 ) ? 0xFF : 0x00;
	 	
  }
}

void send_hid() {
	update_state_joy(g_button_state);
	joy_report_t report = {0};
	generate_report_joy(&report);
	
	if (tud_hid_ready())
	{
		tud_hid_n_report(0x00, 0x00, &report, sizeof(report));
	}
}
/**
 * Initialize Board Pins
 **/
void init() {
	stdio_init_all();

	// LED Pin on when connected
	gpio_init(25);
	gpio_set_dir(25, GPIO_OUT);
	gpio_put(25, 1);   
	
	  for (int i=0; i<NUM_LIGHTS; i++)
  {
    //gpio_init(g_led_pin[i]);
    //gpio_set_dir(g_led_pin[i], GPIO_OUT);
	gpio_set_function(g_led_pin[i], GPIO_FUNC_PWM);
  }

  for (int i=0; i<NUM_BUTTONS; i++)
  {
    gpio_init(g_but_pin[i]);
    gpio_set_dir(g_but_pin[i], GPIO_IN);
	gpio_pull_up(g_but_pin[i]);
  }
}

/**
 * Main Loop Function
 **/
int main(void) {
		
	init();
	
	board_init();
    gpio_put(25, 1);   
	tusb_init();

	while (1) {
		tud_task();
		update_inputs();
		send_hid();
		update_lights_reactive();
		update_lights();
		sleep_us(1000);
	}
	
	return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const* buffer,
                           uint16_t bufsize) {
	(void)itf;
  
	bool report_id_ok = (report_id == REPORTID_LIGHT_OUTPUT);
	if (report_type == HID_REPORT_TYPE_OUTPUT && report_id_ok)
	{
		g_hid_last = board_millis();
		memcpy(g_lamp_state, buffer, NUM_LIGHTS);
	}
	else if (report_type == HID_REPORT_TYPE_FEATURE)
	{
		/* someday */
	}
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t* buffer,
                               uint16_t reqlen) {
	(void)itf;
	(void)report_id;
	(void)report_type;
	(void)buffer;
	(void)reqlen;

	return 0;
}
