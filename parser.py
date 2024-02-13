import argparse
import mimetypes
import os
import stat
from datetime import datetime


def get_entry_info(entry_path):
    st = os.stat(entry_path)
    access_time = datetime.fromtimestamp(st[stat.ST_ATIME]).strftime('%Y-%m-%d-%H:%M:%S')
    owner = os.getlogin()
    mimetype, _ = mimetypes.guess_type(entry_path)
    mimetype = (mimetype or 'inode/directory').replace('/', '.')
    name = os.path.basename(entry_path)
    if name == '':
        name = "root"
    return name, owner, access_time, mimetype

def get_file_info(path):
    # {id} {parent_id} {name} {owner} {access_time} {mimetype}
    files_info = {}
    ids = 0

    files_info[ids] = [ids, -1] + list(get_entry_info(path))
    ids += 1

    def process_folder(folder_path, parent_id=0):
        nonlocal files_info
        nonlocal ids

        try:
            for entry in os.scandir(folder_path):
                entry_path = os.path.join(folder_path, entry.name)

                entry_info = get_entry_info(entry_path)

                files_info[ids] = [ids, parent_id] + list(entry_info)
                ids += 1
                if entry.is_dir():
                    process_folder(entry_path, parent_id=ids-1)
        except PermissionError:
            print(f"Permission denied for {folder_path}")

        return files_info

    all_files_info = process_folder(path)
    print("Finished processing all files and folders")
    print(len(all_files_info))
    print(f"Total ids: {ids}")

    with open('init.txt', 'w') as f:
        for file_id, file_info in all_files_info.items():
            f.write(' '.join(map(str, file_info)) + '\n')

def main():
    parser = argparse.ArgumentParser(description="Recursively gather information about files and folders")
    parser.add_argument("path", help="Path to the directory to analyze")
    args = parser.parse_args()
    get_file_info(args.path)

if __name__ == "__main__":
    main()

