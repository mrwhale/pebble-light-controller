#include "pebble.h"

#define NUM_MENU_SECTIONS 2
//#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 5
#define NUM_SECOND_MENU_ITEMS 5
#define KEY_IP 0
#define KEY_PORT 1
#define KEY_CMD 2
/*
Pebble app for controlling Limitless led lights via the Light-Controler android app
By mrwhale https://github.com/mrwhale
*/
// todo add the ability to read data from phone. need to read in Zone names and display instead of just "zone x"
//TODO get images from eliot for the icon
//todo upload image and add to be the icon of the app
//todo test in a colour watch (
//todo add the colour image in for colour watches
//todo add feedback for colour watches to change the display of "on" zones to inverse (could do this for aplite?)
//todo: clean up these variable names

static TextLayer *s_weather_layer;
static Window *s_main_window;
static MenuLayer *s_menu_layer;
//static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;
static int s_current_icon = 0;
enum Settings { setting_screen = 1, setting_date, setting_vibrate };

/* 
Function that receieves data from the config.js script. This script is responsible for getting any
user defined variables and pass them back to the main app
*/
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char ip_buffer[8];
  static char port_buffer[32];
  static char weather_layer_buffer[32];

  // Read tuples for data
  Tuple *ip_tuple = dict_find(iterator, KEY_IP);
  Tuple *port_tuple = dict_find(iterator, KEY_PORT);
  
  APP_LOG(1, "ip tuple from js %s", ip_tuple->value->cstring);
  APP_LOG(1, "port tuple received from js %s", port_tuple->value->cstring);
  
  
  
  
  // If all data is available, use it
  //if(ip_tuple && port_tuple) {
    snprintf(ip_buffer, sizeof(ip_buffer), "%dC", (int)ip_tuple->value->int32);
    snprintf(port_buffer, sizeof(port_buffer), "%dC", (int)port_tuple->value->int32);

    // Assemble full string and display
    //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", ip_buffer, port_buffer);
    //text_layer_set_text(s_weather_layer, weather_layer_buffer);
  //}
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

/* 
function that listens to which menu item is selected. Used to see which light the user wants to manipultae
*/
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
   DictionaryIterator* dictionaryIterator = NULL;
   app_message_outbox_begin (&dictionaryIterator);
   APP_LOG(APP_LOG_LEVEL_INFO, "cell_section %d", cell_index->section);
   APP_LOG(APP_LOG_LEVEL_INFO, "cell_index %d", cell_index->row);
  // TODO add some feedback in the UI to show that you have pressed the button. invert colours? maybe pebble has a default view 
  switch (cell_index->section){
    case 0:
      switch (cell_index->row) {
      // This is the menu item with the cycling icon
      case 0:
        //send command to phone to toggle colour zone 0
         dict_write_uint8 (dictionaryIterator, 2, 0);
        break;
      case 1:
        // Send command to phone to toggle colour zone 1
         dict_write_uint8 (dictionaryIterator, 2, 1);
        break;
      case 2:
        //send command to phone to toggle colour zone 2
         dict_write_uint8 (dictionaryIterator, 2, 2);
        break;
      case 3:
        //send command to phone to toggle colour zone 3
         dict_write_uint8 (dictionaryIterator, 2, 3);
        break;
      case 4:
        //send command to phone to toggle colour zone 4
         dict_write_uint8 (dictionaryIterator, 2, 4);
         break;
      }
    break;
    case 1:
    switch (cell_index->row){
      case 0:
        //send command to phone to toggle white zone 0
         dict_write_uint8 (dictionaryIterator, 2, 9);
        break;
      case 1:
        //send command to phone to toggle white zone 1
         dict_write_uint8 (dictionaryIterator, 2, 5);
        break;
      case 2:
        //send command to phone to toggle white zone 2
         dict_write_uint8 (dictionaryIterator, 2, 6);
        break;
      case 3:
        //send command to phone to toggle white zone 3
         dict_write_uint8 (dictionaryIterator, 2, 7);
        break;
      case 4:
        //send command to phone to toggle white zone 4
         dict_write_uint8 (dictionaryIterator, 2, 8);
      break;
     }
    break;
   }
   dict_write_end (dictionaryIterator);
   app_message_outbox_send ();
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

/** Old code for when I was trying to make an app that did it all itself
static void sendBroadcastPacket {
    // Open a socket
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd<=0) {
      APP_LOG_LEVEL_INFO, "Error: Could not open socket"");
        return;
    }
    
    // Set socket options
    // Enable broadcast
    int broadcastEnable=1;
    int ret=setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
      APP_LOG_LEVEL_INFO, "Error: Could not open set socket to broadcast mode");
        close(sd);
        return;
    }
    
    // Since we don't call bind() here, the system decides on the port for us, which is what we want.    
    
    // Configure the port and ip we want to send to
    struct sockaddr_in broadcastAddr; // Make an endpoint
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "239.255.255.250", &broadcastAddr.sin_addr); // Set the broadcast IP address
    broadcastAddr.sin_port = htons(1900); // Set port 1900
    
    // Send the broadcast request, ie "Any upnp devices out there?"
    char *request = "M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1900\r\nMAN:\"ssdp:discover\"\r\nST:ssdp:all\r\nMX:1\r\n\r\n";
    ret = sendto(sd, request, strlen(request), 0, (struct sockaddr*)&broadcastAddr, sizeof broadcastAddr);
    if (ret<0) {
      APP_LOG_LEVEL_INFO, "Error: Could not open send broadcast");
        close(sd);
        return;        
    }
    
    // Get responses here using recvfrom if you want...
    close(sd);
}
*/

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
