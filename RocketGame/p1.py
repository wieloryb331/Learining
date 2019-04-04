import pygame
import sys
from rocket import Rocket
from circle import Circle


class Game:

    box = pygame.Rect(10, 50, 50, 50)

    def __init__(self):
        # Config
        self.tps_max = 100.0

        # Initialisation
        pygame.init()
        self.resolution = (1024, 600)
        self.screen = pygame.display.set_mode(self.resolution)
        self.tps_clock = pygame.time.Clock()
        self.tps_delta = 0.0

        self.player = Rocket(self)
        self.circles = [Circle(self, self.player)]

        while True:
            # Handle events
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    sys.exit(0)

            # Ticking
            self.tps_delta += self.tps_clock.tick() / 1000.0
            while self.tps_delta > 1 / self.tps_max:
                self.tick()
                self.tps_delta -= 1 / self.tps_max
            # Drawing
            self.screen.fill((0, 0, 0))
            self.draw()
            pygame.display.flip()

    def tick(self):

        self.player.tick()
        for circle in self.circles:
            circle.tick()


    def draw(self):
        self.player.draw()
        for circle in self.circles:
            circle.draw()

if __name__ == "__main__":
    Game()