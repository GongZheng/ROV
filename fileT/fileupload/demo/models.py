from __future__ import unicode_literals

from django.db import models

class filetoken(models.Model):
    token = models.CharField(max_length = 32)


