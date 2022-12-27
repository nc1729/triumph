../../build/tas -v -i hello_world.tas -o hello_world.tri > verbose_out.txt

python3 ../../tools/disk_writer.py ascii_tilemap.tri hello_world.tri hello_world.td