cd /usr/local/.wb
nohup ./round_v2.py >> round.log 2>&1 &
nohup ./upload.py -i /usr/local/.log/  -o /usr/local/.log/  >> upload.log 2>&1 &
