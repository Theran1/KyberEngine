#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL.h"

enum class main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = main_states::MAIN_CREATION;
	Application* App = NULL;

	while (state != main_states::MAIN_EXIT)
	{
		switch (state)
		{
		case main_states::MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = main_states::MAIN_START;
			break;

		case main_states::MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = main_states::MAIN_EXIT;
			}
			else
			{
				state = main_states::MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case main_states::MAIN_UPDATE:
		{
			update_status update_return = App->Update();

			if (update_return == update_status::UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = main_states::MAIN_EXIT;
			}

			if (update_return == update_status::UPDATE_STOP)
				state = main_states::MAIN_FINISH;
		}
			break;

		case main_states::MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = main_states::MAIN_EXIT;

			break;

		}
	}

	delete App;
	LOG("Exiting application '%s'...\n", TITLE);
	return main_return;
}