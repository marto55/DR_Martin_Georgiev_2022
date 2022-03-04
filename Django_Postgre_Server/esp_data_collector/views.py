from django.http import JsonResponse
import json
from esp_data_collector.models import Reading
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def index(request):
    
    return JsonResponse({"status" : "successfull"})

@csrf_exempt
def handle_esp_reading(request):
    if request.method == "POST":
        try:
            current = json.loads(request.body)["current"]
            voltage = json.loads(request.body)["voltage"]
            temperature = json.loads(request.body)["temperature"]
            wattage = current * voltage
        
            my_reading = Reading()
            my_reading.current = current
            my_reading.voltage = voltage
            my_reading.wattage = wattage
            my_reading.temperature = temperature
            my_reading.save()

            return JsonResponse({"status" : "created"})
        except:
            return JsonResponse({"status" : "failed"})
    else:
        return JsonResponse({"status" : "failed"})