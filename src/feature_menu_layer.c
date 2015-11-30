#include "pebble.h"

#define NUM_MENU_SECTIONS 2
//#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 5
#define NUM_SECOND_MENU_ITEMS 5
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
static TextLayer *s_weather_layer;

static Window *s_main_window;
static MenuLayer *s_menu_layer;
//static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;
static int s_current_icon = 0;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];

  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, KEY_CONDITIONS);

  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
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

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 0", "Select to toggle", NULL);
          break;
        case 1:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "Zone 1", "Select to toggle",NULL);
          break;
        case 2: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 2", "Select to toggle", NULL);
          break;
        case 3: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 3", "Select to toggle", NULL);
          break;
        case 4: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 4", "Select to toggle", NULL);
          break;
        case 5:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 0", "Select to toggle", NULL);
          break;
        case 6:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "Zone 1", "Select to toggle",NULL);
          break;
        case 7: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 2", "Select to toggle", NULL);
          break;
        case 8: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 3", "Select to toggle", NULL);
          break;
        case 9: 
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Zone 4", "Select to toggle", NULL);
          break;
      }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
   DictionaryIterator* dictionaryIterator = NULL;
   app_message_outbox_begin (&dictionaryIterator);
   dict_write_uint8 (dictionaryIterator, 1, 0);
   // dict_write_cstring (dictionaryIterator, 1, "ABCD-1234"); // this fails!
   dict_write_end (dictionaryIterator);
   app_message_outbox_send ();
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 0:
      //send command to phone to toggle colour zone 0
      break;
    case 1:
      // Send command to phone to toggle colour zone 1
      break;
    case 2:
      //send command to phone to toggle colour zone 2
      break;
    case 3:
      //send command to phone to toggle colour zone 3
      break;
    case 4:
      //send command to phone to toggle colour zone 4
      break;
    case 5:
      //send command to phone to toggle white zone 0
      break;
    case 6:
      //send command to phone to toggle white zone 1
      break;
    case 7:
      //send command to phone to toggle white zone 2
      break;
    case 8:
      //send command to phone to toggle white zone 3
      break;
    case 9:
      //send command to phone to toggle white zone 4
      
      break;
  }

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

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
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
