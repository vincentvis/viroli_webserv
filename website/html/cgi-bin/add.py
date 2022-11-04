import cgi, cgitb
import os
import sys

cgitb.enable()
input_data = cgi.FieldStorage()
try:
    num1 = int(input_data.getvalue("val1"))
    num2 = int(input_data.getvalue("val2"))
except:
    print("Status: 404")
    raise SystemExit(0)

print("Content-Type: text/html") # HTML is following
print()                          # Leave a blank line


print("""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
""")


print("<h1>Result of your addition</h1>")
print("<h2>{0} + {1} = {2}</h2>".format(num1, num2, num1 + num2))
print("<br><a href=\"/cgi-bin/post/\">Try another POST</a>, or <a href=\"/cgi-bin/\">Try another GET</a>")

print("""</body>
</html>""")
