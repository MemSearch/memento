from django.http.response import HttpResponse
from django.shortcuts import render
from django.core.paginator import Paginator
from .models import Tag, Profile, MemPic


def paginate(objects, request, per_page = 10):
    paginator = Paginator(objects, per_page)
    page = request.GET.get('page')
    return paginator.get_page(page)


def index(request):
    return render(request, 'index.html', {})


def meme(request):
    return render(request, 'meme.html', {})


def ex(request):
    return render(request, 'ex.html', {})


# def tag(request, id):
#     tag = Tag.objects.get(id=id)
#     questions = Question.objects.tag(tag)
#     questions_per_page = paginate(questions, request, 10)
#     content = {
#         'tag' : tag,
#         'questions_count' : questions.count,
#         'questions': questions_per_page
#     }
#     return render(request, 'tag.html', content)



# def settings(request):
#     return render(request, 'settings.html', {})
