# Generated by Django 4.0.3 on 2022-03-03 16:38

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='reading',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('current', models.FloatField()),
                ('vlotage', models.FloatField()),
                ('wattage', models.FloatField()),
                ('temperature', models.FloatField()),
            ],
        ),
    ]
