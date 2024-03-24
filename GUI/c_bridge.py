import ctypes as ct
import tkinter as tk
import os
import sys
system = sys.platform
if system == 'win32':
    path = os.path.join(os.path.dirname(__file__), '../engine/cmake-build-debug/', 'libengine.dll')
elif system == 'linux':
    path = os.path.join(os.path.dirname(__file__), '../engine/cmake-build-debug/', 'libengine.so')
else:
    raise Exception("Unsupported platform")
try:
    engine = ct.CDLL(path)
except OSError as e:
    tk.messagebox.showerror("Error", f"Could not load the C library: {e}")
    raise e

class Rabbit(ct.Structure):
    pass

class Wolf(ct.Structure):
    pass

class Island(ct.Structure):
    _fields_ = [
        ('first_rabbit', ct.POINTER(Rabbit)),
        ('first_wolf', ct.POINTER(Wolf)),    
        ('day', ct.c_int)
    ]


engine.engine_init.argtypes = [ct.c_int]
engine.engine_init.restype = ct.POINTER(Island)

engine.engine_tick.argtypes = [ct.POINTER(Island)]
engine.engine_tick.restype = None

engine.engine_deinit.argtypes = [ct.POINTER(Island)]
engine.engine_deinit.restype = None

Wolf._fields_ = [
    ('prev', ct.POINTER(Wolf)),
    ('next', ct.POINTER(Wolf)),
    ('location_x', ct.c_uint16),
    ('location_y', ct.c_uint16),
    ('age', ct.c_uint16),
    ('gender', ct.c_int),
    ('is_pregnant', ct.c_bool),
    ('pregnancy_remaining', ct.c_uint16),
    ('starving_time', ct.c_uint16),
    ('offspring_count', ct.c_uint16 ),
    ('has_eaten', ct.c_bool),
    ('is_adult', ct.c_bool),
    ('is_gay', ct.c_bool),
    ('died', ct.c_bool),
    ('partner', ct.POINTER(Wolf)),
    ('first_child', ct.POINTER(Wolf)),

]

Rabbit._fields_ = [
    ('location_x', ct.c_uint16),
    ('location_y', ct.c_uint16),

    ('age', ct.c_uint16),
    ('gender', ct.c_int),

    ('is_pregnant', ct.c_bool),
    ('pregnancy_remaining', ct.c_uint16),
    ('fed_weeks', ct.c_uint16),
    ('is_adult', ct.c_bool),
    ('prev', ct.POINTER(Rabbit)),
    ('next', ct.POINTER(Rabbit))
]


