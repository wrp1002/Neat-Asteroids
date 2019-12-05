#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include <Windows.h>
#include <unordered_map>
#include <random>
#include <time.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "Globals.h"
#include "Neat.h"
#include "ConnectionGene.h"
#include "NodeGene.h"
#include "Agent.h"
#include "GameManager.h"

const float Globals::PI = 3.14159265358979;

using namespace std;

//	TODO:	let them see better					DONE
//	TODO:	Fix graphs							DONE
//	TODO:	Fitness								
//	TODO:	let Player play						DONE
//	TODO:	View best from generations			DONE
//	TODO:	Memory								DONE?
//	TODO:	Stale species						
//	TODO:	Toggle drawing stuff				DONE
//	TODO:	Include velocity of asteroids?		

vector<string> SeparateCommand(string cmd) {
	istringstream ss(cmd);
	vector<string> parts;

	do {
		string token;
		ss >> token;
		parts.push_back(token);

	} while (ss);

	return parts;
}

int main() {
	//string cmd = "python Resources/Graph.py names 1 6 4 7 5 4";
	//system(cmd.c_str());

	srand(time(0));
	bool done = false;
	bool redraw = false;
	bool training = true;
	int framesSinceDraw = 0;
	int subTimer = 200;
	int consistentTimer = 0;

	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_mouse();
	al_install_keyboard();

	ALLEGRO_DISPLAY *display = al_create_display(Globals::screenWidth, Globals::screenHeight);
	al_set_window_title(display, "NEAT Asteroids");
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_FONT *font = al_load_font("Resources/arial.ttf", 12, NULL);

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	Neat neat(vector<string>{"Eye1", "Eye2", "Eye3", "Eye4", "Eye5", "Eye6", "Eye7", "Eye8", "Direction", "CanShoot", "VelX", "VelY", "Mem"}, vector<string>{"Forward", "Left", "Right", "Shoot", "Mem"}, 1000);
	GameManager gm(neat.GetNextAgent());

	Agent *tmpShip = new Agent(neat.NewGenome());
	GameManager interactiveGm(tmpShip);
	vector<Agent *> replayShips;
	int replayIndex = 0;

	al_start_timer(timer);


	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			framesSinceDraw++;
			if (framesSinceDraw > 100 && neat.GetSpeed() > 10) {
				consistentTimer = 0;
				subTimer--;
				if (subTimer <= 0) {
					subTimer = 200;
					neat.SetSpeed(neat.GetSpeed() - 10);
				}
			}

			if (framesSinceDraw == 1 && neat.GetSpeed() > 50) {
				consistentTimer++;
				if (consistentTimer > 100) {
					consistentTimer = 0;
					neat.SetSpeed(neat.GetSpeed() + 10);
				}
			}
			else if (framesSinceDraw > 1 && neat.GetSpeed() > 50)
				consistentTimer = 0;

			if (training) {
				for (int i = 0; i < neat.GetSpeed(); i++) {
					gm.Update();
					neat.Update();
					if (gm.Done()) {
						if (!neat.GenerationDone())
							gm.Reset(neat.GetNextAgent());
						else {
							neat.Evolve();
							neat.Print();
							gm.Reset(neat.GetNextAgent());
						}
					}
				}
			}
			else {
				interactiveGm.Update();
				if (interactiveGm.Done()) {
					if ((unsigned)replayIndex < replayShips.size() - 1) {
						delete replayShips[replayIndex];
						replayShips[replayIndex] = nullptr;

						replayIndex++;
						tmpShip = replayShips[replayIndex];

						if (tmpShip != nullptr) {
							training = false;
							interactiveGm.Reset(tmpShip);
							interactiveGm.SetPlayerControlled(false);
						}
					}
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			interactiveGm.SetKey(ev.keyboard.keycode, true);

			if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				al_stop_timer(timer);
				SetForegroundWindow(GetConsoleWindow());
				string input = "";
				printf("Enter command: ");
				getline(cin, input);

				vector<string> cmd = SeparateCommand(input);
				
				if (cmd[0] == "speed") {
					if (cmd.size() > 1)
						neat.SetSpeed(stoi(cmd[1].c_str()));
				}
				else if (cmd[0] == "graph") {
					string option = "all";
					if (cmd.size() > 1)
						option = cmd[1];
					neat.GraphHistory(option);
				}
				else if (cmd[0] == "play") {
					training = false;
					if (tmpShip != nullptr) {
						delete tmpShip;
						tmpShip = nullptr;
					}
					tmpShip = new Agent(neat.NewGenome());
					interactiveGm.Reset(tmpShip);
					interactiveGm.SetPlayerControlled(true);
					if (cmd.size() > 1 && cmd[1] == "safe")
						interactiveGm.SetSpawnAsteroids(false);
				}
				else if (cmd[0] == "train") {
					training = true;

					for (unsigned i = 0; i < replayShips.size(); i++) {
						if (replayShips[i] != nullptr) {
							delete replayShips[i];
							replayShips[i] = nullptr;
						}
					}

					replayShips.clear();
				}
				else if (cmd[0] == "toggle" || cmd[0] == "draw") {
					if (cmd.size() > 1) {
						neat.ToggleDrawOption(cmd[1]);
					}
				}
				else if (cmd[0] == "view" || cmd[0] == "watch") {
					if (cmd.size() > 1) {
						int index = stoi(cmd[1].c_str());
						if (tmpShip != nullptr) {
							delete tmpShip;
							tmpShip = nullptr;
						}

						replayShips = neat.GetHistoryAgents(index);
						if (replayShips.size() > 0) {
							replayIndex = 0;
							tmpShip = replayShips[replayIndex];

							if (tmpShip != nullptr) {
								training = false;
								interactiveGm.Reset(tmpShip);
								interactiveGm.SetPlayerControlled(false);
							}
						}
					}
				}

				SetForegroundWindow(FindWindow(NULL, "NEAT Asteroids"));
				al_start_timer(timer);
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			interactiveGm.SetKey(ev.keyboard.keycode, false);
		}
		

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			framesSinceDraw = 0;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (training)
				gm.Draw();
			else
				interactiveGm.Draw();

			neat.DrawInfo(font);

			if (replayShips.size() > 0)
				al_draw_textf(font, al_map_rgb(255, 255, 255), Globals::screenWidth / 2, 18, ALLEGRO_ALIGN_CENTER, "Species: %i", replayIndex);

			al_flip_display();
		}
	}
	

	al_destroy_font(font);
	al_destroy_display(display);
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_shutdown_primitives_addon();
}