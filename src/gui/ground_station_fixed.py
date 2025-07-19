import sys
import serial
from PyQt5.QtWidgets import (
    QApplication, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QTextEdit,
    QProgressBar, QGroupBox, QGridLayout, QFrame, QTableWidget
)
from PyQt5.QtGui import QPixmap, QFont
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtMultimedia import QSound
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
from PyQt5.QtWidgets import QTableWidgetItem

class GroundStationModern(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("GROUND STATION CONTROL CENTER")
        self.setGeometry(100, 100, 1400, 750)
        self.setStyleSheet("background-color: #1e1e1e; color: white;")
        self.widgets = {}
        self.graph_data = {"Altitude": [], "Temperature": [], "Pressure": []}
        self.graph_axes = {}
        self.graph_canvases = {}
        self.alarm = QSound("alarm.wav")
        self.alarm_playing = False
        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout()
        main_layout.addWidget(self.create_header())

        body_layout = QHBoxLayout()
        body_layout.addLayout(self.left_column(), 2)
        body_layout.addWidget(self.center_column(), 4)
        body_layout.addLayout(self.right_column(), 3)

        main_layout.addLayout(body_layout)
        self.setLayout(main_layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_ui_from_file)
        self.timer.start(1000)
        self.update_ui_from_file()

    def create_header(self):
        header = QLabel("GELİŞİM UNIVERSITY             Mission Control Dashboard           FALAK TEAM 644183 ")
        header.setStyleSheet("background-color: #333; padding: 10px;")
        header.setAlignment(Qt.AlignCenter)
        header.setFont(QFont("Consolas", 16, QFont.Bold))
        return header

    def left_column(self):
        layout = QVBoxLayout()

        self.widgets["status"] = QTextEdit()
        self.widgets["status"].setReadOnly(True)
        self.widgets["status"].setStyleSheet("background-color: #292929; border: 1px solid #555;")
        layout.addWidget(self.group_box("Status Panel", self.widgets["status"]))

        self.widgets["telemetry"] = QTextEdit()
        self.widgets["telemetry"].setReadOnly(True)
        self.widgets["telemetry"].setStyleSheet("background-color: #808080;")
        layout.addWidget(self.group_box("Telemetry", self.widgets["telemetry"]))

        self.widgets["battery"] = QProgressBar()
        self.widgets["battery"].setStyleSheet("QProgressBar::chunk {background: #0f0;} QProgressBar {text-align: center;}")
        layout.addWidget(self.group_box("Battery Level", self.widgets["battery"]))

        return layout

    def center_column(self):
        container = QVBoxLayout()

        graph_row = QHBoxLayout()
        for label in ["Altitude", "Temperature", "Pressure"]:
            fig, ax = plt.subplots(figsize=(3, 2))
            ax.set_title(label)
            canvas = FigureCanvas(fig)
            self.graph_axes[label] = ax
            self.graph_canvases[label] = canvas
            graph_row.addWidget(canvas)
        container.addLayout(graph_row)

        self.widgets["table"] = QTableWidget(1, 7)
        self.widgets["table"].setHorizontalHeaderLabels([
            "Packet", "Status", "Altitude", "Temp", "Speed", "Battery", "Time"
        ])
        self.widgets["table"].verticalHeader().setVisible(False)
        self.widgets["table"].setEditTriggers(QTableWidget.NoEditTriggers)
        self.widgets["table"].setStyleSheet("""
            QTableWidget { background-color: #222; color: white; gridline-color: #555; }
            QHeaderView::section { background-color: #444; color: yellow; }
        """)
        container.addWidget(self.group_box("Telemetry Overview", self.widgets["table"]))

        wrapper = QWidget()
        wrapper.setLayout(container)
        return wrapper

    def right_column(self):
        layout = QVBoxLayout()
        self.widgets["errors"] = []

        video_placeholder = QLabel("Live Feed Placeholder")
        video_placeholder.setAlignment(Qt.AlignCenter)
        video_placeholder.setStyleSheet("background-color: #444; padding: 10px; border: 1px solid #666;")
        layout.addWidget(self.group_box("Live Camera Feed", video_placeholder))

        error_labels = [
            "Combined Descent", "Payload Descent", "Pressure Missing",
            "GPS Missing", "No Separation", "Command Invalid"
        ]

        error_grid = QGridLayout()
        for i in range(2):
            for j in range(3):
                idx = i * 3 + j
                frame = QFrame()
                frame.setFixedSize(40, 40)
                label = QLabel(error_labels[idx])
                label.setStyleSheet("color: white; font-size: 10px;")
                box = QVBoxLayout()
                box.addWidget(frame, alignment=Qt.AlignCenter)
                box.addWidget(label, alignment=Qt.AlignCenter)
                container = QWidget()
                container.setLayout(box)
                self.widgets["errors"].append((frame, label))
                error_grid.addWidget(container, i, j)

        error_box = QWidget()
        error_box.setLayout(error_grid)
        layout.addWidget(self.group_box("System Warnings", error_box))

        map_label = QLabel()
        pixmap = QPixmap("map_placeholder.jpg")
        if not pixmap.isNull():
            map_label.setPixmap(pixmap.scaled(350, 200, Qt.KeepAspectRatio))
        else:
            empty_img = QPixmap(350, 200)
            empty_img.fill(Qt.gray)
            map_label.setPixmap(empty_img)
            map_label.setText("No Map Image")
            map_label.setAlignment(Qt.AlignCenter)
            map_label.setStyleSheet("color: white;")

        layout.addWidget(self.group_box("Live Location", map_label))
        return layout

    def group_box(self, title, widget):
        group = QGroupBox(title)
        group.setStyleSheet("QGroupBox { font-weight: bold; }")
        vbox = QVBoxLayout()
        vbox.addWidget(widget)
        group.setLayout(vbox)
        return group

    def read_data_file(self):
        if not hasattr(self, 'ser'):
            self.ser = serial.Serial("COM3", 115200, timeout=1)
        ser = self.ser
        field_names = [
            "pressure1", "temp", "altitude1",
            "gps_lat", "gps_lon", "gps_alt",
            "pitch", "roll", "yaw"
        ]

        data = {"status": "", "telemetry": {}, "error_bits": "000000", "raw": "", "fields": {}}

        try:
            if ser.in_waiting:
                line = ser.readline().decode("utf-8").strip()
                data["raw"] = line
                parts = [p.strip() for p in line.split("|")]
                if len(parts) != len(field_names):
                    return data

                v = dict(zip(field_names, parts))
                data["fields"] = v
                data["telemetry"] = v
                data["status"] = "Live Telemetry from COM3"

        except Exception as e:
            print(f"[ERROR] {e}")

        return data

    def update_ui_from_file(self):
        data = self.read_data_file()
        v = data["telemetry"]

        # تحديث حالة الاتصال
        self.widgets["status"].setPlainText(data["status"])

        # تحديث التيليمتري النصي
        telemetry_text = "\n".join([
            f"Pressure: {v.get('pressure1', '---')} Pa",
            f"Temperature: {v.get('temp', '---')} °C",
            f"Altitude: {v.get('altitude1', '---')} m",
            f"GPS Latitude: {v.get('gps_lat', '---')}",
            f"GPS Longitude: {v.get('gps_lon', '---')}",
            f"GPS Altitude: {v.get('gps_alt', '---')} m",
            f"Pitch: {v.get('pitch', '---')}°",
            f"Roll: {v.get('roll', '---')}°",
            f"Yaw: {v.get('yaw', '---')}°"
        ])
        self.widgets["telemetry"].setPlainText(telemetry_text)

        # تحديث الرسوم البيانية
        try:
            alt = float(v.get("altitude1", "0"))
            temp = float(v.get("temp", "0"))
            pres = float(v.get("pressure1", "0"))
        except Exception as e:
            print(f"[GRAPH ERROR] {e}")
            alt, temp, pres = 0, 0, 0

        graph_inputs = {"Altitude": alt, "Temperature": temp, "Pressure": pres}
        for label, val in graph_inputs.items():
            self.graph_data[label].append(val)
            if len(self.graph_data[label]) > 30:
                self.graph_data[label].pop(0)

            ax = self.graph_axes[label]
            ax.clear()
            ax.plot(self.graph_data[label], color='cyan')
            ax.set_title(label)
            self.graph_canvases[label].draw()

        # تحديث الجدول
        row_position = self.widgets["table"].rowCount()
        self.widgets["table"].insertRow(row_position)

        self.widgets["table"].setItem(row_position, 0, QTableWidgetItem(str(row_position + 1)))  # Packet #
        self.widgets["table"].setItem(row_position, 1, QTableWidgetItem(data.get("status", "---")))
        self.widgets["table"].setItem(row_position, 2, QTableWidgetItem(v.get("altitude1", "---")))
        self.widgets["table"].setItem(row_position, 3, QTableWidgetItem(v.get("temp", "---")))
        self.widgets["table"].setItem(row_position, 4, QTableWidgetItem(v.get("gps_alt", "---")))
        self.widgets["table"].setItem(row_position, 5, QTableWidgetItem("---"))  # Battery (فضها أو اربطها)
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = GroundStationModern()
    window.show()
    sys.exit(app.exec_())
