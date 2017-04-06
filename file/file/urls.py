"""file URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin
from one import views as filetest

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^$', filetest.index),
    url(r'^upload', filetest.upload),
    url(r'^upload2/', filetest.upload2),
    url(r'^tk', filetest.get_token),
    url(r'^file', filetest.file_index),
    url(r'^video', filetest.video),
    url(r'^wificonfig', filetest.wifi),
    url(r'^settingwifi', filetest.settingwifi),
    url(r'startpicam',filetest.startpicam),
    url(r'closepicam',filetest.closepicam),
]
