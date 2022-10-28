#include "function.h"
#include <ncurses.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>

const char *MORSE[27] =
  {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.."
  };

const int DOT  = 300;
const int LINE = 900;

const int ESC = 27;
const int TAB =  9;

static int kill    = false;
static int stop    = false;
static int clean   = false;

static sf::SoundBuffer buffer;
static sf::Sound sound;

static void destroy();

static void initReader(Queue *queue);

static void initDisplayer(Queue *queue);

static void initWriter(Queue *queue);

static void *runReader(void *ptr);

static void *runDisplayer(void *ptr);

static void *runWriter(void *ptr);

static void sleep(int msec);

void init(Queue *queue)
{
  initscr();

  refresh();

  initDisplayer(queue);

  initWriter(queue);

  initReader(queue);

  atexit(destroy);
}

static void destroy()
{
  endwin();
}

int wait()
{
  while (!clean)
    continue;

  return !kill;
}

static void initReader(Queue *queue)
{
  pthread_t thread = {};

  pthread_create(&thread, nullptr, runReader, queue);
}

static void initDisplayer(Queue *queue)
{
  pthread_t thread = {};

  pthread_create(&thread, nullptr, runDisplayer, queue);
}

static void initWriter(Queue *queue)
{
  pthread_t thread = {};

  pthread_create(&thread, nullptr, runWriter, queue);
}

static void *runReader(void *ptr)
{
  Queue *queue = (Queue *)ptr;

  if (!buffer.loadFromFile("sound.wav"))
      {
        kill    = true;

        return nullptr;
      }

  sound.setBuffer(buffer);

  while (!kill)
    {
      if (stop)
        continue;

      if (queue_size(queue))
        {
          char ch = queue_pop(queue);

          if (isalpha(ch))
            {
              const char *str = MORSE[tolower(ch) - 'a'];

              while (*str)
                {
                  if (*str == '.')
                    {
                      sound.play();
                      sleep(DOT);
                      sound.stop();
                    }
                  else
                    {
                      sound.play();
                      sleep(LINE);
                      sound.stop();
                    }

                  ++str;

                  sleep(1000);
                }
            }
            else if (ch == ' ')
              sleep(3*LINE);
        }
    }

  return nullptr;
}

static void *runDisplayer(void *ptr)
{
  Queue *queue = (Queue *)ptr;

  clean = false;

  printf("\033[?25l");

  while (!kill)
    {
      dumpQueue(queue);

      for (size_t i = 0; i < queue->capacity / 32 + 1; ++i)
        printf("\033[5A\r");

      fflush(stdout);

      sleep(100);
    }

  printf("\033[?25h");

  clean = true;

  return nullptr;
}

static void *runWriter(void *ptr)
{
  Queue *queue = (Queue *)ptr;

  while (true)
    {
      int ch = getch();

      if (ch == ESC || queue_size(queue) == queue_capacity(queue) - 1)
        {
          kill = true;

          return nullptr;
        }
      else if (ch == TAB)
        {
          stop = !stop;

          system("clear");
        }
      else
        queue_push(queue, (char)ch);
    }

  return nullptr;
}

static void sleep(int msec)
{
  clock_t delay = msec * CLOCKS_PER_SEC / 1000;

  clock_t now = clock();

  while (clock() - now < delay)
    continue;
}
