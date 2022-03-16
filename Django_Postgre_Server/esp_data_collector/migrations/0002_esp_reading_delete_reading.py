# Generated by Django 4.0.3 on 2022-03-15 14:01

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('esp_data_collector', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='esp_reading',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('current', models.FloatField()),
                ('voltage', models.FloatField()),
                ('wattage', models.FloatField()),
                ('temperature', models.FloatField()),
                ('time', models.DateTimeField(auto_now_add=True)),
            ],
        ),
        migrations.DeleteModel(
            name='Reading',
        ),
    ]
