import sys

import pygame 


WINDOW_DIMENSIONS = (500, 500)
SIZE = 20
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)


def read_file():
    with open('result.txt', 'r') as file:
        return [
            (''.join(char) for idx, char in enumerate(line.replace('\n', '')) \
                if not(idx % 2)) for line in file.readlines()[5:]
        ]


def display(surface, lines):
    for row, line in enumerate(lines):
        for column, char in enumerate(line):
            pygame.draw.rect(
                surface, 
                [WHITE, BLACK][char == '#'], 
                pygame.Rect((column + 1) * SIZE, (row + 1) * SIZE, SIZE, SIZE)
            )


if __name__ == '__main__':
    pygame.init()

    surface = pygame.display.set_mode(WINDOW_DIMENSIONS)
    lines = read_file()

    surface.fill(WHITE)
    display(surface, lines)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
        pygame.display.update()
