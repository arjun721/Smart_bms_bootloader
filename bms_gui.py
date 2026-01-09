import serial
import struct
import threading
import tkinter as tk
from tkinter import ttk

# === Serial Config ===
ser = serial.Serial('COM10', 9600, timeout=1)

CELL_NUM = 17
BYTES_PER_CELL = 2
TOTAL_BYTES = CELL_NUM * BYTES_PER_CELL

# === Settings ===
FONT_SIZE = 10
BAR_LENGTH = 130
PAD_X = 6
PAD_Y = 3

# === GUI Setup ===
root = tk.Tk()
root.title("Cell + BMS Monitor System")
root.configure(bg="white")

value_labels = []
level_labels = []
progress_bars = []
status_labels = {}
fet_labels = {}

# === Layout Frame for Cells ===
frame = tk.Frame(root, bg="white")
frame.pack(padx=10, pady=8)

# === Create Cell Displays ===
def create_cell_display(frame, index, row, col):
    label = tk.Label(frame, text=f"C{index + 1}", font=("Arial", FONT_SIZE), bg="white")
    label.grid(row=row * 3, column=col, padx=PAD_X, pady=(PAD_Y, 0), sticky='w')

    level_label = tk.Label(frame, text="---- mV", font=("Arial", FONT_SIZE, "bold"), fg="blue", bg="white")
    level_label.grid(row=row * 3 + 1, column=col, padx=PAD_X, pady=(0, 1), sticky='w')
    level_labels.append(level_label)

    progress = ttk.Progressbar(frame, orient='horizontal', length=BAR_LENGTH, mode='determinate', maximum=5000)
    progress.grid(row=row * 3 + 2, column=col, padx=PAD_X, pady=(0, PAD_Y), sticky='w')
    progress_bars.append(progress)
    value_labels.append(level_label)

# Arrange 6 cells per row
for i in range(CELL_NUM):
    group_row = i // 6
    group_col = i % 6
    create_cell_display(frame, i, group_row, group_col)

# === Status Flags ===
status_flags = {
    15: "cadc_trigger_flag", 14: "cc_overflow_flag", 13: "spi_crc_err_flag",
    12: "porn_flag", 11: "iotp_flag", 10: "cb_timeout_flag", 9: "loadoff_flag",
    8: "loadon_flag", 7: "chgrin_flag", 6: "spiwdt_flag", 5: "trigger_scan_flag",
    4: "ovp_flag", 3: "cocp_flag", 2: "doc1p_flag", 1: "doc2p_flag", 0: "scp_flag",
}

status_frame = tk.LabelFrame(root, text="Status Flags", bg="white", font=("Arial", 11, "bold"))
status_frame.pack(fill="x", padx=10, pady=6)

for i, bit in enumerate(sorted(status_flags.keys(), reverse=True)):
    label = tk.Label(status_frame, text=status_flags[bit], bg="white", fg="gray", font=("Arial", 9), anchor="w")
    label.grid(row=i // 2, column=i % 2, sticky="w", padx=8, pady=2)
    status_labels[bit] = label

# === FET Control Bits ===
fet_frame = tk.LabelFrame(root, text="FET Control Bits", bg="white", font=("Arial", 11, "bold"))
fet_frame.pack(fill="x", padx=10, pady=6)

fet_fields = ["pre_set", "sw_load_detect_enable", "sw_chgr_detect_enable",
              "pre_sel", "sw_chg_ctrl", "sw_dsg_ctrl"]

for i, name in enumerate(fet_fields):
    row = i // 2
    col = (i % 2) * 2
    label = tk.Label(fet_frame, text=f"{name.replace('_', ' ').capitalize()}: ", bg="white", font=("Arial", 9))
    label_val = tk.Label(fet_frame, text="--", bg="white", fg="blue", font=("Arial", 9, "bold"))
    label.grid(row=row, column=col, sticky="w", padx=10, pady=2)
    label_val.grid(row=row, column=col + 1, sticky="w", padx=10, pady=2)
    fet_labels[name] = label_val

# === System Current ===
current_frame = tk.Frame(root, bg="white")
current_frame.pack(fill="x", padx=10, pady=6)

current_label_title = tk.Label(current_frame, text="System Current:", font=("Arial", 10), bg="white")
current_label_title.pack(side="left")

system_current_label = tk.Label(current_frame, text="-- A", font=("Arial", 10, "bold"), fg="darkgreen", bg="white")
system_current_label.pack(side="left", padx=10)

# === GUI Update Functions ===
def update_cells(cell_voltages):
    for i in range(CELL_NUM):
        voltage = cell_voltages[i]
        level_labels[i].after(0, level_labels[i].config, {'text': f"{voltage} mV"})
        progress_bars[i].after(0, progress_bars[i].config, {'value': voltage})

def update_status_fet(status1x, fetCtrlx):
    for bit, label in status_labels.items():
        label.config(fg="green" if (status1x & (1 << bit)) else "gray")

    pre_set = (fetCtrlx >> 8) & 0x7F
    sw_load_detect_en = (fetCtrlx >> 7) & 0x01
    sw_chgr_detect_en = (fetCtrlx >> 6) & 0x01
    pre_sel = (fetCtrlx >> 2) & 0x03
    sw_chg_ctrl = (fetCtrlx >> 1) & 0x01
    sw_dsg_ctrl = (fetCtrlx >> 0) & 0x01

    fet_labels["pre_set"].config(text=f"{pre_set:07b} ({pre_set})")
    fet_labels["sw_load_detect_enable"].config(text="ON" if sw_load_detect_en else "OFF")
    fet_labels["sw_chgr_detect_enable"].config(text="ON" if sw_chgr_detect_en else "OFF")
    fet_labels["pre_sel"].config(text={
        0b01: "01 (Pre-charge)",
        0b10: "10 (Pre-discharge)"
    }.get(pre_sel, f"{pre_sel:02b} (Disabled)"))
    fet_labels["sw_chg_ctrl"].config(text="ON" if sw_chg_ctrl else "OFF")
    fet_labels["sw_dsg_ctrl"].config(text="ON" if sw_dsg_ctrl else "OFF")

def update_current(current_float):
    system_current_label.config(text=f"{current_float:.2f} A")

# === Serial Communication Thread ===
def serial_thread():
    while True:
        byte = ser.read(1)
        if byte == b'\xAA':
            data = ser.read(TOTAL_BYTES)
            if len(data) == TOTAL_BYTES:
                cell_voltages = struct.unpack('<17H', data)
                root.after(0, update_cells, cell_voltages)
        elif byte == b'\xAB':
            data = ser.read(4)
            if len(data) == 4:
                status1x, fetCtrlx = struct.unpack('<HH', data)
                root.after(0, update_status_fet, status1x, fetCtrlx)
        elif byte == b'\xAC':
            data = ser.read(4)
            if len(data) == 4:
                current = struct.unpack('<f', data)[0]
                root.after(0, update_current, current)

# Start serial thread
threading.Thread(target=serial_thread, daemon=True).start()
root.mainloop()
