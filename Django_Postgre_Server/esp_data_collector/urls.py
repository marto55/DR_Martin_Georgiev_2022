
from django.urls import path
from . import views

urlpatterns = [
    path('esp_handler', views.handle_esp_reading, name='esp_handler'),
]