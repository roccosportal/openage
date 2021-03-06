import zlib

from struct import Struct, unpack_from
from util import dbg, file_get_path, file_open, zstr

endianness = '< '

class Empires2X1P1:
	"""represents the compressed empires2_x1_p1.dat"""

	def __init__(self, fname):
		self.fname = fname
		dbg("reading empires2_x1_p1 from %s..." % fname, 1)

		fname = file_get_path(fname, write = False)
		f = file_open(fname, binary = True, write = False)

		dbg("decompressing data from %s" % fname, 1)

		compressed_data = f.read()
		#decompress content with zlib (note the magic -15)
		#-15: - -> there is no header, 15 is the max windowsize
		self.content = zlib.decompress(compressed_data, -15)
		f.close()

		compressed_size   = len(compressed_data)
		decompressed_size = len(self.content)

		del compressed_data

		dbg("length of compressed data: %d = %d kB" % (compressed_size, compressed_size/1024), 1)
		dbg("length of decompressed data: %d = %d kB" % (decompressed_size, decompressed_size/1024), 1)


		from util import file_write
		print("saving uncompressed dat file...")
		file_write(file_get_path('info/empires2x1p1.raw', write=True), self.content)

		#the main data storage
		self.data = dict()

		offset = 0
		offset = self.read(self.content, offset)

		dbg("finished reading empires*.dat at %d of %d bytes (%f%%)." % (offset, decompressed_size, 100*(offset/decompressed_size)), 1)

	def read(self, raw, offset):

		#char versionstr[8];
		header_struct = Struct(endianness + "8s")
		header = header_struct.unpack_from(raw, offset)
		offset += header_struct.size

		self.data["version"] = zstr(header[0])

		dbg("dat version: %s" % (self.data["version"]), 1)

		from gamedata import terrain
		t = terrain.TerrainHeaderData()
		offset = t.read(raw, offset)
		self.data["terrain"] = t.data

		from gamedata import playercolor
		t = playercolor.PlayerColorData()
		offset = t.read(raw, offset)
		self.data["color"] = t.data

		from gamedata import sound
		t = sound.SoundData()
		offset = t.read(raw, offset)
		self.data["sound"] = t.data

		from gamedata import graphic
		t = graphic.GraphicData()
		offset = t.read(raw, offset)
		self.data["graphic"] = t.data

		from gamedata import terrain
		t = terrain.TerrainData(self.data["terrain"]["terrain_count"])
		offset = t.read(raw, offset)
		self.data["terrain"].update(t.data)

		tmp_struct = Struct(endianness + "438c")
		offset += tmp_struct.size

		from gamedata import terrain
		t = terrain.TerrainBorderData()
		offset = t.read(raw, offset)
		self.data["terrain"].update(t.data)

		from gamedata import tech
		t = tech.TechData()
		offset = t.read(raw, offset)
		self.data["tech"] = t.data

		from gamedata import unit
		t = unit.UnitHeaderData()
		offset = t.read(raw, offset)
		self.data["unit"] = t.data

		from gamedata import civ
		t = civ.CivData()
		offset = t.read(raw, offset)
		self.data["civ"] = t.data

		from gamedata import research
		t = research.ResearchData()
		offset = t.read(raw, offset)
		self.data["research"] = t.data

		tmp_struct = Struct(endianness + "7i")
		offset += tmp_struct.size

		from gamedata import tech
		t = tech.TechtreeData()
		offset = t.read(raw, offset)
		self.data["tech"] = t.data

		#self.data = dict()
		#import pprint
		#pprint.pprint(t.data)

		return offset


	def __str__(self):
		ret = "[age2x1p1]\n"
		ret += str(self.data)
		return ret


def test(datfile):
	dbg("converting the empires2_x1_p1 main game data file...")
	tryit = Empires2X1P1(datfile)
	#import pprint
	#dbg("result:\n" + pprint.pformat(tryit.data), 1)
	res = str(tryit.data)

	destfile = "/tmp/empires2_x1_p1.py"
	with open(destfile, "w") as f:
		f.write(res)

	print("written to %s" % destfile)
	#print(res)

if __name__ == "__main__":
	#fak u frank
	import os
	os.system("python convert -o ../../ -d -vv ../../Age_of_Empires_Files/")

