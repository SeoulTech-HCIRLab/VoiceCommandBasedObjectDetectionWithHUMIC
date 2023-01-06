import re

def check_pattern(str):

    dd = ['right', 'left', 'y middle']
    for d in dd:
        print(d)
        d2 = f'.*{d}.*'

        if re.match(d2, str):
            print("Pass")
        else:
            print("Fail")

ee = 'aaa y middle eeeeeeee'
check_pattern(ee)


