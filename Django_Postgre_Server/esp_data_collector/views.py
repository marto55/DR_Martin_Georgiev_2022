from django.http import JsonResponse
import json
from esp_data_collector.models import Reading
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def handle_esp_reading(request):
    if request.method == "POST":
        try:
            current = json.loads(request.body)["current"]
            voltage = json.loads(request.body)["voltage"]
            temperature = json.loads(request.body)["temperature"]
            wattage = round( current * voltage, 2) 
        
            my_reading = Reading()
            my_reading.current = current
            my_reading.voltage = voltage
            my_reading.wattage = wattage
            my_reading.temperature = temperature
            my_reading.save()

            return JsonResponse({"message" : "created"}, status=201)
        except:
            return JsonResponse({"message" : "failed"}, status=500)
    else:
        return JsonResponse({"message" : "failed"}, status=500)



        