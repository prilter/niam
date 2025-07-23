import subprocess
import os

import notes


def num_to_files(n: str):
    res = []

    n = n.replace(".", "").replace(" ", "")
    for dig in n:
        res.append(f"notes/{notes.NOTE_FREQS[dig]}.mp3")
        print(f"{dig} - {notes.NOTE_FREQS[dig]}", end=" ")
        
    return res 

def files_to_music(fls):
    # Создать текстовый файл со списком для ffmpeg
    with open("fls_programm", "w") as f:
        for fl in fls:
            f.write(f"file '{fl}'\n")

    # Запустить ffmpeg через стандартный subprocess
    subprocess.run(
    [
        "ffmpeg",
        "-f", "concat",
        "-safe", "0",
        "-i", "fls_programm",
        "-c", "copy",
        "output.mp3"
    ],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL
    )

    os.remove("fls_programm")

def num_to_music(n: str): 
    print("The Start")
    fls = num_to_files(n)
    files_to_music(fls)
    print("The End")
