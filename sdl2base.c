#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

static struct
{
	int framecount;
	float rate;
	unsigned int calc_start;
} fps;

int quit = 0;
int in_inited = 0;

SDL_Window *win = NULL;
SDL_Renderer *rend = NULL;
SDL_Texture *tex = NULL;
unsigned short *pix = NULL;


void
Quit (void)
{
	if (pix != NULL)
	{
		free (pix);
		pix = NULL;
	}

	if (tex != NULL)
	{
		SDL_DestroyTexture (tex);
		tex = NULL;
	}

	if (rend != NULL)
	{
		SDL_DestroyRenderer (rend);
		rend = NULL;
	}

	if (win != NULL)
	{
		SDL_DestroyWindow (win);
		win = NULL;
	}

	if (in_inited)
	{
		SDL_QuitSubSystem (SDL_INIT_EVENTS);
		in_inited = 0;
	}

	SDL_QuitSubSystem (SDL_INIT_TIMER);
	SDL_Quit ();
	exit (EXIT_SUCCESS);
}


int
main (int argc, const char *argv[])
{
	Uint32 flags;
	SDL_Event sdlev;

	if (SDL_Init(SDL_INIT_TIMER) != 0)
	{
		fprintf (stderr, "ERROR: SDL init failed: %s\n", SDL_GetError());
		exit (EXIT_FAILURE);
	}

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0)
	{
		fprintf (stderr, "ERROR: failed initializing events: %s\n", SDL_GetError());
		Quit ();
	}
	in_inited = 1;

	flags = 0;
	if (0)
		flags |= SDL_WINDOW_FULLSCREEN;
	if ((win = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, flags)) == NULL)
	{
		fprintf (stderr, "ERROR: failed creating window: %s\n", SDL_GetError());
		Quit ();
	}

	if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL)
	{
		fprintf (stderr, "ERROR: failed creating renderer: %s\n", SDL_GetError());
		Quit ();
	}

	if ((tex = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 640, 480)) == NULL)
	{
		fprintf (stderr, "ERROR: failed creating texture: %s\n", SDL_GetError());
		Quit ();
	}

	pix = malloc (640 * 480 * 2);

	while (!quit)
	{
		while (SDL_PollEvent(&sdlev))
		{
			int k = sdlev.key.keysym.sym;

			switch (sdlev.type)
			{
			case SDL_KEYUP:
				if (k == 'f')
					printf ("%g\n", fps.rate);
				break;

			//...

			case SDL_QUIT:
				quit = 1;
				break;

			default:
				break;
			}
		}
		{
			int i;
			for (i = 0; i < 480; i++)
				pix[(i * 640) + i] = 0xffff;
		}

		SDL_UpdateTexture (tex, NULL, pix, 640 * sizeof(*pix));
		SDL_RenderCopy (rend, tex, NULL, NULL);
		SDL_RenderPresent (rend);

		/* calculate the framerate */
		fps.framecount++;
		{
			unsigned int now = SDL_GetTicks ();
			if ((now - fps.calc_start) > 250)
			{
				fps.rate = fps.framecount / ((now - fps.calc_start) / 1000.0);
				fps.framecount = 0;
				fps.calc_start = now;
			}
		}
	}

	Quit ();

	return 0;
}
