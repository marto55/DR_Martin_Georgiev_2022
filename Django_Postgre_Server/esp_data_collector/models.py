from django.db import models

class reading(models.Model):
    current = models.FloatField()
    vlotage = models.FloatField()
    wattage = models.FloatField()
    temperature = models.FloatField()