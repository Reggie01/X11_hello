#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>



int main()
{

	const int board_width = 400;
	const int board_height = 400;	
	
    Display *display;
    Window window;
    int screen;
	char buf[256];
	char buf_8[8];
	int alphabet_conversion;
	char * key_to_string = 0;
	Drawable board;
	GC gc;
	XGCValues gc_values;
	int rect_x = 0;
	int rect_y = 0;
	int rect_width = 50;
	int rect_height = 50;
	Status expose_evt;
	XExposeEvent re_expose;
	XEvent my_evt;
	
	
    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

	//XIM input_method = XOpenIM(display, NULL, NULL, NULL);
	//XIC input_context = XCreateIC( input_method);	

	

    // Get the default screen number
    screen = DefaultScreen(display);

	printf("Screen: %i\n", screen);

    // Create a window
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                           10, 10, board_width, board_height, 1,
                           //WhitePixel(display, screen),
                           BlackPixel(display, screen),
							WhitePixel(display, screen));

    // Set the window title
    XStoreName(display, window, "Hello, World!");

    // Map the window to the screen and display it
    XMapWindow(display, window);
    XFlush(display);

	// Get the window attributes
	XWindowAttributes xwa;
	XGetWindowAttributes( display, window, &xwa );
/*
	printf("Window #: %li\n", window);
	printf("Window x: %i\n", xwa.x);
	printf("Window y: %i\n", xwa.y);
	printf("Window width: %i\n", xwa.width);
	printf("Window height: %i\n", xwa.height);
*/

	//Listen for events in XCreateWindow or XChangeWindowAttributes or XSelectInput
	// Default events are ignored
	XSelectInput( display, window, ButtonPress | KeyPress | KeyPressMask |ExposureMask );
	
    // Wait for user input
    XEvent event;
	
	gc_values.background = 0x000000;
	gc_values.foreground = 0xff0000;
	gc = XCreateGC( display,window, GCForeground|GCBackground, &gc_values);
	
	// temp timer
	int cnt = 0;
	
	// Creat XExpose event
	my_evt.type = Expose;
	my_evt.xexpose.window = window;
	my_evt.xexpose.x = 10;
	my_evt.xexpose.y = 10;
	my_evt.xexpose.width = board_width;
	my_evt.xexpose.height = board_height;
	//my_evt.xexpose.send_event = 1;
	//re_expose.window = window;
	//re_expose.x = 10;
	//re_expose.y = 10;
	//re_expose.width = board_width;
	//re_expose.height = board_width;
	//my_evt.type = XExposeEvent;
	//my_evt.xexpose = re_expose;
	
    while (1) {
        XNextEvent(display, &event);
		
		printf("Event type: %i\n", event.type);
		if( event.type == ButtonPress ) {			
			printf( "button type: %i\n", event.xbutton.type);
			printf("Coordinates in event window button.x: %i\n", event.xbutton.x);
			printf("Coordinates in event window button.y: %i\n", event.xbutton.y);
			printf("Coordinates in root window button.x: %i\n", event.xbutton.x_root);
			printf("Coordinates in root window button.x: %i\n", event.xbutton.y_root);
			printf("button detail: %i\n", event.xbutton.button);
			
		} else if( event.type == KeyPress) {
			printf("key type: %i\n", event.xkey.type);			
			printf("key evt: %i\n", event.xkey.keycode);
			printf("Coordinate in root window xkey.x: %i\n",event.xkey.x);
			printf("Coordinate in root window xkey.y: %i\n", event.xkey.y);
			printf( "Keycode: %i\n", event.xkey.keycode );
			printf( "State: %i\n", event.xkey.state );
			//printf( "Key to string: %c\n", event.xkey.

			//alphabet_conversion = XwcLookupString(input_context, &event.xkey, &buf, sizeof(buf), NULL, NULL);
			//alphabet_conversion = XLookupString( &event.xkey, buf_8, sizeof(buf_8), NULL, NULL);			
			/*
			if( alphabet_conversion > 0 ) {
				printf("Keycode maps to: %c\n", buf[0]);
			}
		*/
			KeySym letter = XLookupKeysym( &event.xkey, 0);
			switch( letter ){
				case 'a':
					printf("letter 'a' was pressed\n");
					break;
				default:
					key_to_string = XKeysymToString( letter );
					printf("XKeysymToString: %c\n",  key_to_string[0]);
					printf("Letter: %lu\n", letter);
					/*if(IsModifierKey(letter) ){
						printf("This is a modifier key");
					}*/				
			}
			
		} else if( event.type == Expose ) {
			XClearWindow(display, window);
			//++rect_x;	
			++cnt;
			if( cnt == 100 ) {
					++rect_y;
					cnt = 0;
			}
			printf("cnt: %i\n", cnt);
			printf("rect_y: %i\n", rect_y);
			XDrawRectangle(display, window, gc, rect_x, rect_y, rect_width, rect_height);
			expose_evt = XSendEvent( display, window, 0, ExposureMask, &my_evt);
		}
		
	}
    // Clean up	

	//XCloseIM(input_method);
	//XDestroyIC( input_context );
	
	XFreeGC( display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

