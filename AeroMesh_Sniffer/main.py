import serial
from colorama import Fore, Back, Style
import datetime
import argparse
import re

PORT = None

INFO_COLOR = Fore.LIGHTGREEN_EX
STATUS_COLOR = Fore.LIGHTCYAN_EX
COMMAND_COLOR = Fore.MAGENTA
SENSOR_COLOR = Fore.LIGHTBLUE_EX

SRC_COLOR = Back.LIGHTBLUE_EX
DST_COLOR = Back.LIGHTGREEN_EX
BROADCAST_COLOR = Back.LIGHTCYAN_EX

devices_list = {}

def next_color():
    colors = [x for x in dir(Back) if x[0] != "_" and x not in ["BLACK", "RESET"]]
    return getattr(Back, colors[len(devices_list)])

def print_line(text: str, timestamp: datetime.datetime, log_file_ptr, last_timestamp: datetime.datetime):
    parts = text.split("\n")[0].split(" ")
    
    delta = timestamp - last_timestamp

    src = ""
    dst = "broadcast"
    msg = ""

    for p in parts:
        if "src" in p:
            src = p.split(":")[1]
            if not src in devices_list.keys():
                devices_list[src] = next_color()            
        elif "msg" in p:
            msg = p.split(":")[1]
        elif "dst" in p:
            dst = p.split(":")[1]
        else:
            msg = f"{msg} {p}"

    msg_parts = msg.split(";")
    if len(msg_parts) > 1:
        msg_type = msg_parts[0]
        sub_type = msg_parts[1]
        data = msg_parts[2:]

        msg_type_color = Fore.WHITE
        if "info" in msg_type: msg_type_color = INFO_COLOR
        elif "status" in msg_type: msg_type_color = STATUS_COLOR
        elif "comm" in msg_type: msg_type_color = COMMAND_COLOR
        elif "gps" in msg_type or "imu" in msg_type or "load" in msg_type: msg_type_color = SENSOR_COLOR

        sub_t_str = f"({sub_type})" if sub_type != "0" else ""
        dst_color = BROADCAST_COLOR if "broadcast" in dst else DST_COLOR

        data_str = f"{data}" if data else ""

        printable_str = f"[{timestamp}]{Fore.LIGHTBLACK_EX}({str(delta/datetime.timedelta(microseconds=1000))+')':<11}{Style.RESET_ALL} "\
                        f"src: {devices_list[src]}{Fore.BLACK}{src:>10}{Style.RESET_ALL} "\
                        f"dst: {dst_color}{Fore.BLACK}{dst:>10}{Style.RESET_ALL} "\
                        f"msg: {msg_type_color}{msg_type}{sub_t_str} {data_str}{Style.RESET_ALL}"
    elif src:
        dst_color = BROADCAST_COLOR if "broadcast" in dst else DST_COLOR
        printable_str = f"[{timestamp}]{Fore.LIGHTBLACK_EX}({str(delta/datetime.timedelta(microseconds=1000))+')':<11}{Style.RESET_ALL} "\
                        f"src: {devices_list[src]}{Fore.BLACK}{src:>10}{Style.RESET_ALL} "\
                        f"dst: {dst_color}{Fore.BLACK}{dst:>10}{Style.RESET_ALL} "\
                        f"msg: {Back.LIGHTYELLOW_EX}{Fore.BLACK}{msg}{Style.RESET_ALL}"
    else: 
        printable_str = f"[{timestamp}]{Fore.LIGHTBLACK_EX}({str(delta/datetime.timedelta(microseconds=1000))+')':<11}{Style.RESET_ALL} "\
                        f"{Back.LIGHTRED_EX}{Fore.BLACK}{text.strip()}{Style.RESET_ALL}"

    if printable_str:
        print(printable_str)
        if log_file_ptr:
            ansi_escape = re.compile(r'(?:\x1B[@-_]|[\x80-\x9F])[0-?]*[ -/]*[@-~]')
            log_file_ptr.write(f"{ansi_escape.sub('', printable_str)}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("port", help="Serial port to monitor", nargs="+")
    parser.add_argument("-l", "--log", help="Log to file as displayed, provide file name.", default=None)
    parser.add_argument("-r", "--lograw", help="Log to provided file the raw data from serial.", default=None)
    

    args = parser.parse_args()
    PORT = args.port[0]

    log_file = None
    if args.log:
        log_file = open(args.log, 'w')

    log_raw = None
    if args.lograw:
        log_raw = open(args.lograw, 'w')

    try:
        mesh_access = serial.Serial(PORT, 115200)

        last_timestamp = datetime.datetime.now()
        while mesh_access.is_open:
            line = str(mesh_access.readline(), "UTF-8")
            now = datetime.datetime.now()
            print_line(line, now, log_file, last_timestamp)
            if log_raw:
                log_raw.write(line)
            last_timestamp = now

        mesh_access.close()
    except PermissionError as e:
        print(f"{Fore.RED}Serial Port Access Denied, make sure the port {PORT} is not used by another software.{Style.RESET_ALL}")
    except serial.SerialException as e:
        print(f"{Fore.RED}Serial Port not found {PORT}, make sure the bridge device is connected and the right port provided.{Style.RESET_ALL}")

    if log_file:
        log_file.close()
    if log_raw:
        log_raw.close()