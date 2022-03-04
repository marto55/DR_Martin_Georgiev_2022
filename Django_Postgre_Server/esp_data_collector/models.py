from django.db import models

class Reading(models.Model):
    current = models.FloatField()
    voltage = models.FloatField()
    wattage = models.FloatField()
    temperature = models.FloatField()