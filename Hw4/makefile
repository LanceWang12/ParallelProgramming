ZIP_FILE_NAME=HW4_310552021.zip
ZIP_PART=part1 part2 url.txt

all: mkdir move zip copy

mkdir:
	mkdir -p part1

move:
	cp part1.1/*.cc part1
	cp part1.2/*.cc part1

zip:
	zip $(ZIP_FILE_NAME) -r $(ZIP_PART)

copy:
	@scp -r ~/HW4 pp10:~/