#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
int current_hour;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  current_hour = tick_time->tm_hour;

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler_hour(struct tm *tick_time) {
    //int theNum = tick_time->tm_hour;
    int theNum = tick_time->tm_min;
    if ( theNum != current_hour){
      vibes_short_pulse();
    }

#ifdef PBL_COLOR   
    int i = theNum % 12;
    if (i == 0){
        window_set_background_color(s_main_window,  GColorWhite );  
    }else if ( i == 1){
        window_set_background_color(s_main_window,  GColorCyan);  
    }else if ( i  == 2){
        window_set_background_color(s_main_window, GColorIcterine  );  
    }else if ( i == 3){
        window_set_background_color(s_main_window, GColorRajah  );  
    }else if ( i == 4){
        window_set_background_color(s_main_window, GColorLightGray  );  
    }else if ( i == 5){
        window_set_background_color(s_main_window, GColorMintGreen  );  
    }else if ( i == 6){
        window_set_background_color(s_main_window, GColorElectricBlue  );  
    }else if ( i == 7){
        window_set_background_color(s_main_window, GColorLiberty   );  
    }else if ( i == 8){
        window_set_background_color(s_main_window, GColorVividCerulean   );  
    }else if ( i == 9){
        window_set_background_color(s_main_window, GColorMelon   );  
    }else if ( i == 10){
       window_set_background_color(s_main_window, GColorClear  );  
    }else if ( i == 11){
       window_set_background_color(s_main_window, GColorBlueMoon   );  
    }      
#else
    int i = theNum % 2;
    if (i == 0){
        window_set_background_color(s_main_window, GColorWhite);
        text_layer_set_text_color(s_time_layer, GColorBlack);        
    }else {      
        window_set_background_color(s_main_window, GColorBlack );
        text_layer_set_text_color(s_time_layer, GColorWhite);      
    }  
#endif
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  tick_handler_hour(tick_time);
  update_time();
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);  
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

