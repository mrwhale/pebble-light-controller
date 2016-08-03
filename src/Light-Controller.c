#include "pebble.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 5
#define NUM_SECOND_MENU_ITEMS 5
#define KEY_ZONE 2
#define KEY_CMD 3
#define KEY_ZONE0 5
#define KEY_ZONE1 6
#define KEY_ZONE2 7
#define KEY_ZONE3 8
#define KEY_ZONE4 9
#define KEY_ZONE5 10
#define KEY_ZONE6 11
#define KEY_ZONE7 12
#define KEY_ZONE8 13
#define KEY_ZONE9 14
#define ZONE1_NAME 15
#define ZONE2_NAME 16
#define ZONE3_NAME 17
#define ZONE4_NAME 18
#define ZONE5_NAME 19
#define ZONE6_NAME 20
#define ZONE7_NAME 21
#define ZONE8_NAME 22
/*
Pebble app for controlling Limitless led lights via the Light-Controler android app
By mrwhale https://github.com/mrwhale
*/
//todo add the colour image in for colour watches
//todo add feedback for colour watches to change the display of "on" zones to inverse (could do this for aplite?)
//static TextLayer *s_weather_layer;
static Window *s_main_window;
static MenuLayer *s_menu_layer;
//static GBitmap *s_menu_icons[NUM_MENU_ICONS];
//static GBitmap *s_background_bitmap;
//static int s_current_icon = 0;
enum Settings { setting_screen = 1, setting_date, setting_vibrate };
//Variables to hold zone names
char zone_zero[15] = "Colour Global";
char zone_one[15] = "Zone 1c";
char zone_two[15] = "Zone 2c";
char zone_three[15] = "Zone 3c";
char zone_four[15] = "Zone 4c";
char zone_nine[15] = "White Global";
char zone_five[15] = "Zone 1w";
char zone_six[15] = "Zone 2w";
char zone_seven[15] = "Zone 3w";
char zone_eight[15] = "Zone 4w";


/* 
Function that receieves data back from the companion android app.
*/
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Just got a message from the phone!");  
  // Store incoming information. Read tuples for data
  Tuple *zone_zero_tuple = dict_find(iterator, KEY_ZONE0);
  Tuple *zone_one_tuple = dict_find(iterator, KEY_ZONE1);
  Tuple *zone_two_tuple = dict_find(iterator, KEY_ZONE2);
  Tuple *zone_three_tuple = dict_find(iterator, KEY_ZONE3);
  Tuple *zone_four_tuple = dict_find(iterator, KEY_ZONE4);
  Tuple *zone_five_tuple = dict_find(iterator, KEY_ZONE5);
  Tuple *zone_six_tuple = dict_find(iterator, KEY_ZONE6);
  Tuple *zone_seven_tuple = dict_find(iterator, KEY_ZONE7);
  Tuple *zone_eight_tuple = dict_find(iterator, KEY_ZONE8);
  Tuple *zone_nine_tuple = dict_find(iterator, KEY_ZONE9);
  Tuple *zone_one_name = dict_find(iterator, ZONE1_NAME);
  Tuple *zone_two_name = dict_find(iterator, ZONE2_NAME);
  Tuple *zone_three_name = dict_find(iterator, ZONE3_NAME);
  Tuple *zone_four_name = dict_find(iterator, ZONE4_NAME);
  Tuple *zone_five_name = dict_find(iterator, ZONE5_NAME);
  Tuple *zone_six_name = dict_find(iterator, ZONE6_NAME);
  Tuple *zone_seven_name = dict_find(iterator, ZONE7_NAME);
  Tuple *zone_eight_name = dict_find(iterator, ZONE8_NAME);
  int size = dict_size(iterator);
  APP_LOG(APP_LOG_LEVEL_INFO, "dict size is %d", size);

  //If we receive zone data from the phone app, lets see what they say about it and store it
  // for later use TODO
  if(zone_zero_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone zero received %d", zone_zero_tuple->value->int8);
  } 
  if(zone_one_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone one received %d", zone_one_tuple->value->int8);
  }
  if(zone_two_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone two received %d", zone_two_tuple->value->int8);
  }
  if(zone_three_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone three received %d", zone_three_tuple->value->int8);
  }
  if(zone_four_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone four received %d", zone_four_tuple->value->int8);
  }
  if(zone_five_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone five received %d", zone_five_tuple->value->int8);
  }
  if(zone_six_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone six received %d", zone_six_tuple->value->int8);
  }
  if(zone_seven_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone seven received %d", zone_seven_tuple->value->int8);
  }
  if(zone_eight_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone eight received %d", zone_eight_tuple->value->int8);
  }
  if(zone_nine_tuple){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone nine received %d", zone_nine_tuple->value->int8);
  }
  //If we receive zone names from android app, lets read them and then save them over original varibale
  // we should also save this to persistant storage for later use plz
  if(zone_one_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone one name received %s", zone_one_name->value->cstring);
    strcpy(zone_one, zone_one_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone one overide %s", zone_one);
  }
  if(zone_two_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone two name received %s", zone_two_name->value->cstring);
    strcpy(zone_two, zone_two_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone two overide %s", zone_two);
  }
  if(zone_three_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone three name received %s", zone_three_name->value->cstring);
    strcpy(zone_three, zone_three_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone three overide %s", zone_three);
  }
  if(zone_four_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone four name received %s", zone_four_name->value->cstring);
    strcpy(zone_four, zone_four_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone four overide %s", zone_four);
  }
  if(zone_five_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone five name received %s", zone_five_name->value->cstring);
    strcpy(zone_five, zone_five_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone five overide %s", zone_five);
  }
  if(zone_six_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone six name received %s", zone_six_name->value->cstring);
    strcpy(zone_six, zone_six_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone six overide %s", zone_six);
  }
  if(zone_seven_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone seven name received %s", zone_seven_name->value->cstring);
    strcpy(zone_seven, zone_seven_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone seven overide %s", zone_seven);
  }
  if(zone_eight_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone eight name received %s", zone_eight_name->value->cstring);
    strcpy(zone_eight, zone_eight_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone eight overide %s", zone_eight);
  }

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Colour Zones");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "White Zones");
      break;
  }
}

/*
Draw the UI to the pebble. This needs lots of work
*/
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in. 2 sections. 1 colour 1 white. each with 5 zones  
  switch (cell_index->section){
    case 0:
      switch (cell_index->row){
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, zone_zero, "Press on, Long off", NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, zone_one, "Press on, Long off", NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, zone_two, "Press on, Long off", NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, zone_three, "Press on, Long off", NULL);
          break;
        case 4:
          menu_cell_basic_draw(ctx, cell_layer, zone_four, "Press on, Long off", NULL);
          break;
      }
      break;
    case 1:
      switch (cell_index->row){      
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, zone_nine, "Press on, Long off", NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, zone_five, "Press on, Long off", NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, zone_six, "Press on, Long off", NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, zone_seven, "Press on, Long off", NULL);
          break;
        case 4:
          menu_cell_basic_draw(ctx, cell_layer, zone_eight, "Press on, Long off", NULL);
          break;
      }
      break;
  } 
}

void send_cmd(int section, int row, int cmd){
  
  DictionaryIterator* dictionaryIterator = NULL;
  app_message_outbox_begin (&dictionaryIterator);
  switch(section){
    case 0:
      //APP_LOG(APP_LOG_LEVEL_INFO,"send_cmd section 0");
      switch(row){
        case 0:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 0);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_zero, cmd);
          break;
        case 1:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 1);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_one, cmd);
          break;
        case 2:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 2);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_two, cmd);
          break;
        case 3:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 3);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_three, cmd);
          break;
        case 4:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 4);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_four, cmd);
          break;        
      }
      break;
    case 1:
      //APP_LOG(APP_LOG_LEVEL_INFO,"send_cmd section 1");
      switch(row){
        case 0:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 9);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_nine, cmd);
          break;
        case 1:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 5);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_five, cmd);
          break;
        case 2:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 6);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_six, cmd);
          break;
        case 3:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 7);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_seven, cmd);
          break;
        case 4:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 8);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
	  APP_LOG(APP_LOG_LEVEL_INFO, "Going to send %s command %d", zone_eight, cmd);
          break;
      }
  }
   dict_write_end (dictionaryIterator);
   //int inbox_size = dict_size(dictionaryIterator);
   //APP_LOG(APP_LOG_LEVEL_INFO,"Outbox dict size before send %d", inbox_size);
   app_message_outbox_send ();
}


/* 
Callback function on long press of select key
This will send the phone app an OFF command for the selected zone in the menu
*/
static void menu_long_click(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    APP_LOG(APP_LOG_LEVEL_INFO,"You pressed select for longk time");
    send_cmd(cell_index->section,cell_index->row,0);
  }
  

/* 
Callback function on short press of select key
This will send the phone app an ON command for the selected zone in the menu
*/
static void menu_short_click(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    APP_LOG(APP_LOG_LEVEL_INFO,"You pressed select for short time");  
    send_cmd(cell_index->section,cell_index->row,1);
}

static void main_window_load(Window *window) {
  // Now we prepare to initialize the menu layer 
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_normal_colors(s_menu_layer, PBL_IF_COLOR_ELSE(GColorIcterine, GColorWhite), GColorBlack);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_short_click,
    .select_long_click = menu_long_click
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
  //gbitmap_destroy(s_background_bitmap);
}

static void init() {
  s_main_window = window_create();  
  window_set_background_color(s_main_window, PBL_IF_COLOR_ELSE(GColorIcterine, GColorWhite));
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
    // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(350, 100);
  //TODO if they dont exist we should consider asking the main app for them

  APP_LOG(APP_LOG_LEVEL_INFO, "Reading Zone names in from storage");
  if(persist_exists(ZONE1_NAME)){
    persist_read_string(ZONE1_NAME, zone_one, 15);
  }
  if(persist_exists(ZONE2_NAME)){
    persist_read_string(ZONE2_NAME, zone_two, 15);
  }
  if(persist_exists(ZONE3_NAME)){
    persist_read_string(ZONE3_NAME, zone_three, 15);
  }
  if(persist_exists(ZONE4_NAME)){
    persist_read_string(ZONE4_NAME, zone_four, 15);
  }

  if(persist_exists(ZONE5_NAME)){
    persist_read_string(ZONE5_NAME, zone_five, 15);
  }
  if(persist_exists(ZONE6_NAME)){
    persist_read_string(ZONE6_NAME, zone_six, 15);
  }
  if(persist_exists(ZONE7_NAME)){
    persist_read_string(ZONE7_NAME, zone_seven, 15);
  }
  if(persist_exists(ZONE8_NAME)){
    persist_read_string(ZONE8_NAME, zone_eight, 15);
  }
}

static void deinit() {
  //Write all zone names to storage on de-init
  APP_LOG(APP_LOG_LEVEL_INFO, "Writing zone names to strorage");
  persist_write_string(ZONE1_NAME, zone_one);
  persist_write_string(ZONE2_NAME, zone_two);
  persist_write_string(ZONE3_NAME, zone_three);
  persist_write_string(ZONE4_NAME, zone_four);

  persist_write_string(ZONE5_NAME, zone_five);
  persist_write_string(ZONE6_NAME, zone_six);
  persist_write_string(ZONE7_NAME, zone_seven);
  persist_write_string(ZONE8_NAME, zone_eight);
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
