import re


def get_file_params(file_name):
    file_regex_str = r"data_N(\d+)_k(\d+)_l(\d+)\.bin"
    res = re.search(file_regex_str, file_name)
    if res is None:
        raise LookupError
    num_keys, k_pt, l_pt = res.groups()

    return int(num_keys), float(k_pt), float(l_pt)
