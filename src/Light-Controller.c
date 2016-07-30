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
/*
Pebble app for controlling Limitless led lights via the Light-Controler android app
By mrwhale https://github.com/mrwhale
*/
// todo add the ability to read data from phone. need to read in Zone names and display instead of just "zone x"
//todo test in a colour watch (
//todo add the colour image in for colour watches
//todo add feedback for colour watches to change the display of "on" zones to inverse (could do this for aplite?)

static TextLayer *s_weather_layer;
static Window *s_main_window;
static MenuLayer *s_menu_layer;
//static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;
static int s_current_icon = 0;
enum Settings { setting_screen = 1, setting_date, setting_vibrate };
//Variables to hold zone names
char zone_zero[] = "Zone 0";
char zone_one[] = "Zone 1w";
char zone_two[] = "Zone 2w";
char zone_three[] = "Zone 3w";
char zone_four[] = "Zone 4w";
char zone_five[] = "Zone 0c";
char zone_six[] = "Zone 1c";
char zone_seven[] = "Zone 2c";
char zone_eight[] = "Zone 3c";
char zone_nine[] = "Zone 4c";

/* 
Function that receieves data back from somewhere. was used when i needed config.js to get user input. dont need that anymore
will repurpose to receive data from companion app 
*/
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  // static char ip_buffer[8];
  //static char port_buffer[32];
  //static char weather_layer_buffer[32];

  APP_LOG(APP_LOG_LEVEL_INFO, "Just got a message from the phone!");  
  // Read tuples for data
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
  if(zone_one_name){
    APP_LOG(APP_LOG_LEVEL_INFO, "Zone one name received %s", zone_one_name->value->cstring);
    //zone_one = zone_one_name->value->cstring;
    strcpy(zone_one, zone_one_name->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "zone one overide %s", zone_one);
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
  // Determine which section we're going to draw in
      // Use the row to specify which item we'll draw   
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
          menu_cell_basic_draw(ctx, cell_layer, zone_five, "Press on, Long off", NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, zone_six, "Press on, Long off", NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, zone_seven, "Press on, Long off", NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, zone_eight, "Press on, Long off", NULL);
          break;
        case 4:
          menu_cell_basic_draw(ctx, cell_layer, zone_nine, "Press on, Long off", NULL);
          break;
      }
      break;
  } 
}

void send_cmd(int section, int row, int cmd){
  
  DictionaryIterator* dictionaryIterator = NULL;
  app_message_outbox_begin (&dictionaryIterator);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "cell_section %d", section);
  APP_LOG(APP_LOG_LEVEL_INFO, "cell_index %d", row);
  switch(section){
    case 0:
      APP_LOG(APP_LOG_LEVEL_INFO,"send_cmd section 0");
      switch(row){
        case 0:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 0);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 1:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 1);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 2:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 2);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 3:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 3);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 4:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 4);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;        
      }
      break;
    case 1:
      APP_LOG(APP_LOG_LEVEL_INFO,"send_cmd section 1");
      switch(row){
        case 0:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 9);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 1:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 5);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 2:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 6);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 3:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 7);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
        case 4:
          dict_write_uint8 (dictionaryIterator, KEY_ZONE, 8);
          dict_write_uint8 (dictionaryIterator, KEY_CMD, cmd);
          break;
      }
  }
   dict_write_end (dictionaryIterator);
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
  // Here we load the bitmap assets
  //s_menu_icons[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BIG_WATCH);
  //s_menu_icons[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  //s_menu_icons[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);

  // And also load the background
  //s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  //icon_layer = bitmap_layer_create(GRect(82, 0, 61, 61));

  //layer_add_child(window_layer, bitmap_layer_get_layer(icon_layer));
  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
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

  // Cleanup the menu icons
  //for (int i = 0; i < NUM_MENU_ICONS; i++) {
    //gbitmap_destroy(s_menu_icons[i]);
  //}

  gbitmap_destroy(s_background_bitmap);
}

static void init() {
  s_main_window = window_create();  
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
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
