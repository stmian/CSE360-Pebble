#include "pebble.h"

  
#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1
#define NUM_STATUS_TYPES 2
#define NUM_EVENT_TYPES 3
  
static Window *window;

// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;

// Menu items can optionally have an icon drawn with them
static GBitmap *menu_icons1[NUM_MENU_ICONS];
static GBitmap *menu_icons2[NUM_MENU_ICONS];
static char *status[NUM_STATUS_TYPES];
static char *event[NUM_EVENT_TYPES];


static int current_icon1 = 0;
static int current_icon2 = 0;
static int status1 = 0;
static int status2 = 0;
static int eventNumber = 0;



// You can draw arbitrary things in a menu item such as a background
static GBitmap *menu_background;

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
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

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "      Track an Activity");
      break;

    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Current Activity: ");
      break;
  }
  }


// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Workout", status[status1], menu_icons1[current_icon1]);
          break;

        case 1:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "Sleep", status[status2] , menu_icons2[current_icon2]);
          break;
        
        
        case 2:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "Export Data", "Select to Export", NULL);
          break;
        
       
        
      //  case 2:
          // Here we use the graphics context to draw something different
          // In this case, we show a strip of a watchface's background
      //    graphics_draw_bitmap_in_rect(ctx, menu_background,
      //        (GRect){ .origin = GPointZero, .size = layer_get_frame((Layer*) cell_layer).size });
      //    break;
      }
      break;

    case 1:
      switch (cell_index->row) {
        case 0:
          // There is title draw for something more simple than a basic menu item
          menu_cell_title_draw(ctx, cell_layer, event[eventNumber]);
          break;
      }
  }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 0:
      // Cycle the status messages
      current_icon1 = (current_icon1 + 1) % NUM_MENU_ICONS;
      status1 = (status1 + 1) % NUM_STATUS_TYPES;
      
      if(current_icon1 % 2 == 1) 
      {
        eventNumber = 1;
        //Code to start tracking workout
        
       time_t time	(	time_t * tloc);	
        
      }
    
      if(current_icon1 % 2 == 0)
      {
        eventNumber = 0;
        //Code to stop tracking workout
      }
      
      
      // After changing the icon, mark the layer to have it updated
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    
    case 1:
      // Cycle the status messages
      current_icon2 = (current_icon2 + 1) % NUM_MENU_ICONS;
      status2 = (status2 + 1) % NUM_STATUS_TYPES;  
     
         
      if(current_icon2 % 2 == 1) 
      {
        eventNumber = 2;
        //Code to start tracking Sleep
        
        
      }
    
      if(current_icon2 % 2 == 0)
      {
        eventNumber = 0;
        //Code to stop tracking sleep
      }
          
      // After changing the icon, mark the layer to have it updated
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;  
    
  }

}

// This initializes the menu upon window load
void window_load(Window *window) {
  // Here we load the bitmap assets
  // resource_init_current_app must be called before all asset loading
  int num_menu1_icons = 0;
  int num_menu2_icons = 0;
  int statusArray = 0;
  int eventArray = 0;
  
  menu_icons1[num_menu1_icons++] = gbitmap_create_with_resource(RESOURCE_ID_blankImage);//RESOURCE_ID_IMAGE_MENU_ICON_BIG_WATCH);
  menu_icons1[num_menu1_icons++] = gbitmap_create_with_resource(RESOURCE_ID_Logo);//RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  menu_icons2[num_menu2_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);
  menu_icons2[num_menu2_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  
  status[statusArray++] = "Press to Start";
  status[statusArray++] = "Press to Stop";
  
  event[0] = "None";
  event[1] = "Workout";
  event[2] = "Sleep";
  
  
  // And also load the background
  menu_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);

  // Cleanup the menu icons
  for (int i = 0; i < NUM_MENU_ICONS; i++) {
    gbitmap_destroy(menu_icons1[i]);
    gbitmap_destroy(menu_icons2[i]); 
  }

  // And cleanup the background
  gbitmap_destroy(menu_background);
}




//Main code for Pebble to run
int main(void) {
  window = window_create();

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
