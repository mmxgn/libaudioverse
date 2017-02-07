import os
import os.path
import sys
import shutil
import subprocess
# Libaudioverse imports
import docgen
import bindings.get_info

if __name__ == '__main__':
    print("Building documentation...")
    info=bindings.get_info.get_all_info()
    root_directory = bindings.get_info.get_root_directory()
    docgen.prepare_docs(info)
    dest_dir = os.path.join(root_directory, 'build', 'documentation')

    node_reference = docgen.make_node_reference(info)
    c_api_docs=docgen.make_c_api(info)
    enum_docs=docgen.make_enumerations(info)
    if os.path.exists(dest_dir):
        shutil.rmtree(dest_dir)

    shutil.copytree(os.path.join(root_directory, 'documentation'), dest_dir)

    with open(os.path.join(dest_dir, 'node_reference.asciidoc'), 'w') as f:
        f.write(node_reference)
    with open(os.path.join(dest_dir, 'c_api.asciidoc'), 'w') as f:
        f.write(c_api_docs)
    with open(os.path.join(dest_dir, 'enumerations.asciidoc'), 'w') as f:
        f.write(enum_docs)

    subprocess.call(["asciidoctor", os.path.join(dest_dir, 'libaudioverse_manual.asciidoc')], stderr= subprocess.STDOUT, shell = True)

    if not os.path.exists(os.path.join(root_directory, "build", "artifacts")):
        os.makedirs(os.path.join(root_directory, "build", "artifacts"))
    shutil.copy(os.path.join(root_directory, "build", "documentation", "libaudioverse_manual.html"),
    os.path.join(root_directory, "build", "artifacts"))
