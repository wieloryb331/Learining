import pygame
from pygame.math import Vector2


class Rocket(object):
    def __init__(self, game):
        self.game = game
        self.speed = 0.6
        self.gravity = 0.0

        self.size = self.game.screen.get_size()

        self.points = []
        self.radius = 10

        self.position = Vector2(self.size[0]/2, self.size[1]/2)
        self.velocity = Vector2(0, 0)
        self.acceleration = Vector2(0, 0)

    def add_force(self, force):
        self.acceleration += force

    def tick(self):
        # Input
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:
            self.add_force(Vector2(-self.speed, 0))
        if keys[pygame.K_w]:
            self.add_force(Vector2(0, -self.speed))
        if keys[pygame.K_s]:
            self.add_force(Vector2(0, self.speed))
        if keys[pygame.K_d]:
            self.add_force(Vector2(self.speed, 0))

        # Physics
        self.velocity *= 0.90
        self.velocity -= Vector2(0, -self.gravity)
        self.velocity += self.acceleration
        self.position += self.velocity
        self.acceleration *= 0

    def draw(self):
        # Base Triangle
        self.points = [Vector2(0, -10), Vector2(5, 5), Vector2(-5, 5)]

        # Rotate Points
        angle = self.velocity.angle_to(Vector2(0, 1))
        self.points = [p.rotate(angle) for p in self.points]

        # Fix y axis
        self.points = [Vector2(p.x, p.y*-1) for p in self.points]

        # Add current position
        self.points = [self.position + p * 3 for p in self.points]

        # Drawing
        pygame.draw.circle(self.game.screen, (120, 120, 120), [int(self.points[0].x), int(self.points[0].y)], self.radius)
        pygame.draw.polygon(self.game.screen, (0, 100, 255), self.points)

        # rect = pygame.Rect(self.position.x, self.position.y, 50, 50)
        # pygame.draw.rect(self.game.screen, (0, 200, 255), rect)
