from django.contrib import admin
from memapp.models import Tag, Profile, MemPic

# Register your models here.

admin.site.register(Tag)
admin.site.register(Profile)
admin.site.register(MemPic)
