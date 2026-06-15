// Joel Adu - Lab 8 Bitmap Movement

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const int SCREEN_W = 900;
const int SCREEN_H = 800;
const int SHIP_SIZE = 64;

int main()
{
    bool done = false;
    bool redraw = true;
    bool moving = true;

    int direction = 1; // 0 up, 1 right, 2 down, 3 left

    float shipX = 100;
    float shipY = 350;

    float dx = 4;
    float dy = 0;

    float angle = 0;

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* ship = NULL;
    ALLEGRO_BITMAP* background = NULL;

    if (!al_init())
        return -1;

    al_install_keyboard();
    al_init_primitives_addon();

    display = al_create_display(SCREEN_W, SCREEN_H);

    if (!display)
        return -1;

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);

    // Create background bitmap
    background = al_create_bitmap(SCREEN_W, SCREEN_H);
    al_set_target_bitmap(background);
    al_clear_to_color(al_map_rgb(5, 5, 25));

    // Simple space background
    for (int i = 0; i < 120; i++)
    {
        int x = rand() % SCREEN_W;
        int y = rand() % SCREEN_H;
        al_draw_filled_circle(x, y, 2, al_map_rgb(255, 255, 255));
    }

    al_draw_filled_circle(760, 120, 55, al_map_rgb(120, 120, 160));
    al_draw_filled_circle(740, 105, 10, al_map_rgb(80, 80, 110));
    al_draw_filled_circle(780, 130, 14, al_map_rgb(80, 80, 110));

    // Create ship bitmap pointing right by default
    ship = al_create_bitmap(SHIP_SIZE, SHIP_SIZE);
    al_set_target_bitmap(ship);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    // Ship body
    al_draw_filled_triangle(8, 8, 8, 56, 60, 32, al_map_rgb(0, 200, 255));
    al_draw_filled_triangle(10, 16, 10, 48, 48, 32, al_map_rgb(120, 0, 255));

    // Window
    al_draw_filled_circle(28, 32, 8, al_map_rgb(255, 255, 0));

    // Engine flame
    al_draw_filled_triangle(8, 24, 8, 40, 0, 32, al_map_rgb(255, 100, 0));

    // Back to screen
    al_set_target_backbuffer(display);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;

            case ALLEGRO_KEY_SPACE:
                moving = false;
                dx = 0;
                dy = 0;
                break;

            case ALLEGRO_KEY_RIGHT:
                moving = true;
                direction = 1;
                dx = 4;
                dy = 0;
                angle = 0;
                break;

            case ALLEGRO_KEY_LEFT:
                moving = true;
                direction = 3;
                dx = -4;
                dy = 0;
                angle = 0;
                break;

            case ALLEGRO_KEY_UP:
                moving = true;
                direction = 0;
                dx = 0;
                dy = -4;
                angle = -3.14 / 2;
                break;

            case ALLEGRO_KEY_DOWN:
                moving = true;
                direction = 2;
                dx = 0;
                dy = 4;
                angle = 3.14 / 2;
                break;
            }
        }

        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

            if (moving)
            {
                shipX += dx;
                shipY += dy;

                if (shipX <= 0)
                {
                    shipX = 0;
                    dx = 4;
                    direction = 1;
                }

                if (shipX >= SCREEN_W - SHIP_SIZE)
                {
                    shipX = SCREEN_W - SHIP_SIZE;
                    dx = -4;
                    direction = 3;
                }

                if (shipY <= 0)
                {
                    shipY = 0;
                    dy = 4;
                    direction = 2;
                    angle = 3.14 / 2;
                }

                if (shipY >= SCREEN_H - SHIP_SIZE)
                {
                    shipY = SCREEN_H - SHIP_SIZE;
                    dy = -4;
                    direction = 0;
                    angle = -3.14 / 2;
                }
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_draw_bitmap(background, 0, 0, 0);

            if (direction == 1)
            {
                al_draw_bitmap(ship, shipX, shipY, 0);
            }
            else if (direction == 3)
            {
                al_draw_bitmap(ship, shipX, shipY, ALLEGRO_FLIP_HORIZONTAL);
            }
            else
            {
                al_draw_rotated_bitmap(
                    ship,
                    SHIP_SIZE / 2,
                    SHIP_SIZE / 2,
                    shipX + SHIP_SIZE / 2,
                    shipY + SHIP_SIZE / 2,
                    angle,
                    0
                );
            }

            al_flip_display();
        }
    }

    al_destroy_bitmap(ship);
    al_destroy_bitmap(background);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}