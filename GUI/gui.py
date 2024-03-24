import elements as e
from c_bridge import *
import ctypes as ct
from ctypes import CDLL, c_int, POINTER, Structure
from tkinter import messagebox, simpledialog
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import NavigationToolbar2Tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import os
import time
import tkinter as tk
import matplotlib

matplotlib.use("TkAgg")
import threading


# Define the GUI application
def add_method(func):
    setattr(RabbitSimulatorApp, func.__name__, func)


class RabbitSimulatorApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Rabbit Population Simulator')
        self.geometry('720x480')
        self.island = None
        self.island_pointer = None
        self.toggle_state = False
        self.day = tk.StringVar(
            value=str(None if not self.island else self.island.day))
        self.rabbit_count = tk.StringVar(value="NULL")
        self.wolf_count = tk.StringVar(value="NULL")
        # Define GUI widgets
        self.seedLabel = tk.Label(self, text='Random Seed:')
        self.seedLabel.pack()

        self.seed_entry = tk.Entry(self)
        self.seed_entry.pack()

        self.startButton = tk.Button(self, text='Set seed',
                                     command=self.start_simulation)
        self.startButton.pack()

        self.roundlabel0 = e.RoundBox(
            text1='Current day:', text2=self.day, width=120, height=50, arc=20, borderwidth=4, color='black')

        self.roundlabel1 = e.RoundBox(
            text1='Wolf count:', text2=self.wolf_count, width=120, height=50, arc=20, borderwidth=4, color='red')

        self.roundlabel2 = e.RoundBox(
            text1='Rabbit count:', text2=self.rabbit_count, width=120, height=50, arc=20, borderwidth=4, color='green')

        self.speed_regulator = tk.Scale(self, from_=100, to=0, orient=tk.VERTICAL, length=200)

        self.days_l = tk.Label(self, textvariable=self.day)
        self.days_l.pack()

        self.adult_rabbits_l = tk.Label(self, text="Current adult rabbits: ")
        self.days = tk.Text(self, state='disabled', height=1, width=5)

    def init_graph(self):
        self.fig = Figure(figsize=(5, 5), dpi=100, frameon=False, facecolor='#32F032')

        self.ax = self.fig.add_subplot(111)
        self.ax.set_xlabel('x')
        self.ax.set_ylabel('y')
        self.ax.spines['right'].set_visible(False)
        self.ax.spines['top'].set_visible(False)
        self.fig.patch.set_visible(False)

        self.ax.axis('off')

        self.ax.set_facecolor('#32F032')
        self.ax.tick_params(axis='x', which='both', bottom=False,
                        top=False, labelbottom=False)

        # Selecting the axis-Y making the right and left axes False
        self.ax.tick_params(axis='y', which='both', right=False,
                        left=False, labelleft=False)

        # Iterating over all the axes in the figure
        # and make the Spines Visibility as False
        # for pos in ['right', 'top', 'bottom', 'left']:
        #     self.ax.gca().spines[pos].set_visible(False)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.draw()
        self.canvas.get_tk_widget().place(x=200, y=0, width=560, height=480)
        self.canvas.get_tk_widget().place(x=200, y=0, width=560, height=480)

    def update_graph(self):
        self.ax.clear()
        self.ax.set_xlabel('x')
        self.ax.set_ylabel('y')
        # self.ax.set_facecolor('blue')
        self.ax.set_facecolor('#32F032')

        # self.ax.set_title('island')
        self.ax.axis('off')
        self.ax.spines['right'].set_visible(False)
        self.ax.spines['top'].set_visible(False)
        self.ax.set_xlim(0, 1000)
        self.ax.set_ylim(0, 1000)
        # self.ax.grid(True)
        self.ax.scatter([one.location_x for one in self.rabbits], [
            one.location_y for one in self.rabbits], c='r', marker='.')
        self.ax.scatter([one.location_x for one in self.wolves], [
            one.location_y for one in self.wolves], c='b', marker=',')
        self.canvas.draw()
        self.canvas.get_tk_widget().place(x=200, y=0, width=560, height=480)
        # self.canvas.get_tk_widget().place(x=200, y=0, width=560, height=480)
        self.ax.set_facecolor('#32F032')

        self.update()

    def get_rabbits(self):
        rabbits = []
        rabbits.append(self.island.first_rabbit.contents)
        i = 0

        while type(rabbits[i].next) == POINTER(Rabbit):
            try:
                rabbits.append(rabbits[i].next.contents)
                i += 1
            except:
                break
        return rabbits

    def get_wolves(self):
        wolfs = []
        wolfs.append(self.island.first_wolf.contents)
        i = 0
        while type(wolfs[i].next) == POINTER(Wolf):
            try:
                wolfs.append(wolfs[i].next.contents)
                i += 1
            except:
                break
        return wolfs

    def toggle_sim(self):
        print(self.main_thread_lock.locked())
        if self.toggle_state:
            self.main_thread_lock.acquire()
            self.runButton.config(text="RUN")
            self.toggle_state = False

        else:
            self.main_thread_lock.release()
            self.runButton.config(text="PAUSE")
            self.toggle_state = True


    def start_simulation(self):
        # Get the seed value from the entry widget
        try:
            seed = int(self.seed_entry.get())
            self.init_graph()

            self.runButton = tk.Button(self, text='RUN', command=self.toggle_sim, width=15, height=5, bg='#6f6ff0',
                                       fg='white')
            self.runButton.place(x=20, y=20)

            self.roundlabel0.place(x=40, y=150)
            self.roundlabel1.place(x=40, y=210)
            self.roundlabel2.place(x=40, y=270)
            self.speed_regulator.place(x=140, y=130)
            self.speed_regulator.set(100)

            # Initialize the simulation
            self.island_pointer = engine.engine_init(seed)
            self.main_thread = threading.Thread(target=self.run_simulation, daemon=True)
            self.main_thread_lock = threading.Lock()
            self.main_thread_lock.acquire()
            self.main_thread.start()

        except ValueError:
            messagebox.showerror("Error", "Please enter a valid integer seed.")
            return

    def exec_one_day(self):

        engine.engine_tick(self.island_pointer)
        self.island = self.island_pointer.contents
        self.rabbits = self.get_rabbits()
        self.wolves = self.get_wolves()
        # self.fig.set_facecolor('green')
        self.day.set(f"Current day: {self.island.day}")
        self.rabbit_count.set(f"{len(self.rabbits)}")
        self.wolf_count.set(f"{len(self.wolves)}")
        print(f"Current day: {self.island.day}")
        self.ax.set_facecolor('#32F032')

        self.update_graph()

    # def exec_one_day(self):
    #
    #     self.rabbit_count.set(f"{self.main_thread_lock.locked()}")
    #     self.ax.set_facecolor('#32F032')
    #     self.update()

    def run_simulation(self):
        not_finished = True
        while not_finished:
            while not self.main_thread_lock.locked():

                try:
                    self.exec_one_day()
                except:
                    not_finished = False
                    break
                time.sleep(0.5- (0.5 * self.speed_regulator.get() / 100))
        print("Simulation finished itself")
        self.finish_screen()

    def finish_screen(self):
        self.startButton.destroy()
        self.roundlabel0.destroy()
        self.roundlabel1.destroy()
        self.roundlabel2.destroy()
        self.runButton.destroy()
        self.seed_entry.destroy()
        self.days_l.destroy()
        self.seedLabel.destroy()
        # tk.Label(self, text='Simulation results').place(x=150, y=0)
        # tk.Label(self, text=f'Days: ').place(x=0, y=30)

    def safe_close(self):
        try:
            engine.engine_deinit(self.island_pointer)
            print("Deinitialized engine")
        except:
            pass
        self.destroy()


if __name__ == '__main__':
    app = RabbitSimulatorApp()
    app.protocol("WM_DELETE_WINDOW", app.safe_close)
    app.mainloop()
