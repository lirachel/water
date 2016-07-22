#include <pebble.h>
#include <math.h>

static Window *s_intro_window, *s_calculate_window;
static TextLayer *s_text_layer, *s_calculate_layer;
//static GFont s_text_font;

static int weight;
static char enterWeight[30];

static int waterNeeded;
static char waterAmount[100];

static void intro_window_load(Window *window){
	
	s_text_layer = text_layer_create(GRect(25, 50, 75, 75));
	text_layer_set_background_color(s_text_layer, GColorBlack);
 	text_layer_set_text_color(s_text_layer, GColorWhite);
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	
	snprintf(enterWeight, sizeof(enterWeight), "Enter your weight: %d", weight);
	text_layer_set_text(s_text_layer, enterWeight);
	
	layer_add_child(window_get_root_layer(s_intro_window), text_layer_get_layer(s_text_layer));
}

static void intro_window_unload(Window *window){
	text_layer_destroy(s_text_layer);
}

static void calculate_water_needed(int weight){
	waterNeeded = weight/2;
	snprintf(waterAmount, sizeof(waterAmount), "You need %d ounces of water a day!", waterNeeded);
	text_layer_set_text(s_calculate_layer, waterAmount);
	layer_add_child(window_get_root_layer(s_calculate_window), text_layer_get_layer(s_calculate_layer));
}

static void calculate_window_load(Window *window){
	s_calculate_layer = text_layer_create(GRect(25, 50, 100, 100));
	text_layer_set_background_color(s_calculate_layer, GColorBlack);
 	text_layer_set_text_color(s_calculate_layer, GColorWhite);
	text_layer_set_font(s_calculate_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_calculate_layer, GTextAlignmentCenter);
	
	calculate_water_needed(weight);
	layer_add_child(window_get_root_layer(s_calculate_window), text_layer_get_layer(s_calculate_layer));
}

static void calculate_window_unload(Window *window){
	
}



static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	// A single click has just occured
	window_stack_push(s_calculate_window, true);
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  // A single click has just occured
	weight ++;
	snprintf(enterWeight, sizeof(enterWeight), "Enter your weight: %d", weight);
	text_layer_set_text(s_text_layer, enterWeight);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  // A single click has just occured
	weight --;
	if(weight<0){
		weight = 0;
	}
	snprintf(enterWeight, sizeof(enterWeight), "Enter your weight: %d", weight);
	text_layer_set_text(s_text_layer, enterWeight);
}

static void click_config_provider(void *context) {
	// Subcribe to button click events here
	//window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);

}

static void init(){
	s_intro_window = window_create();
	s_calculate_window = window_create();
	
	// Use this provider to add button click subscriptions
	window_set_click_config_provider(s_intro_window, click_config_provider);
	
	window_set_window_handlers(s_intro_window, (WindowHandlers) {
    .load = intro_window_load,
    .unload = intro_window_unload
 	});
	
	window_set_background_color(s_intro_window, GColorBlack);
	
	window_set_window_handlers(s_calculate_window, (WindowHandlers) {
    .load = calculate_window_load,
    .unload = calculate_window_unload
 	});
	
	window_set_background_color(s_calculate_window, GColorBlack);
	
	window_stack_push(s_intro_window, true);
}

static void deinit(){
	window_destroy(s_intro_window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}