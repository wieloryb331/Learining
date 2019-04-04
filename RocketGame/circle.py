import pygame
from pygame.math import Vector2
from math import sqrt


class Circle:
    def __init__(self, game, rocket):
        self.game = game
        self.rocket = rocket
        # self.speed = 0.4
        self.gravity = 0.0

        self.size = self.game.screen.get_size()

        self.radius = 30

        self.position = Vector2(self.size[0]/3, self.size[1]/3)
        self.velocity = Vector2(0, 0)
        self.acceleration = Vector2(0, 0)

    def add_force(self, force):
        self.acceleration += force

    def tick(self):
        # Input
        '''keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:
            self.add_force(Vector2(-self.speed, 0))
        if keys[pygame.K_w]:
            self.add_force(Vector2(0, -self.speed))
        if keys[pygame.K_s]:
            self.add_force(Vector2(0, self.speed))
        if keys[pygame.K_d]:
            self.add_force(Vector2(self.speed, 0))'''

        # Physics
        # Collision
        distance = sqrt((self.position.x - self.rocket.points[0].x)**2 + (self.position.y - self.rocket.points[0].y)**2)
        if distance < self.radius + self.rocket.radius:
            self.add_force(self.rocket.velocity*0.8)

        self.velocity *= 0.875
        self.velocity -= Vector2(0, -self.gravity)
        self.velocity += self.acceleration
        self.position += self.velocity
        self.acceleration *= 0

        # Edge Mechanics
        if self.position.x - self.radius < 0 or self.position.x > self.size[0] - self.radius or self.position.y - self.radius < 0 or self.position.y > self.size[1] - self.radius:
            self.position = Vector2(self.size[0]/2, self.size[1]/2)

    def draw(self):
        # Drawing
        pygame.draw.circle(self.game.screen, (100, 100, 100), [int(self.position.x), int(self.position.y)], self.radius)


