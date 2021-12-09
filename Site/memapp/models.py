from django.contrib.auth.models import User
from django.db import models
import datetime

class Tag(models.Model):
    name = models.CharField(max_length=64, unique=True)

    def __str__(self):
        return self.name


class MemPic(models.Model):
    image = models.ImageField(upload_to="uploads/memimg", default="../static/img/oblozka.jpg")
    date = models.DateField(auto_now=False, auto_now_add=True)
    tags = models.ManyToManyField(Tag, related_name="memes", blank=True)


class Profile(models.Model):
    image = models.ImageField(upload_to="uploads/memimg", default="../static/img/oblozka.jpg")
    user = models.OneToOneField(User, on_delete=models.CASCADE)    