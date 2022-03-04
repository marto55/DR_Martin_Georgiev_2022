from django.urls import path
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('esp_handler', views.handle_esp_reading, name='handle_esp_reading'),
]