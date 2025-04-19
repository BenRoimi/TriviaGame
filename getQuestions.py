import requests
import sqlite3

def fetch_questions(amount=50, category=9, difficulty="medium", question_type="multiple", token=None):
    url = f"https://opentdb.com/api.php?amount={amount}&category={category}&difficulty={difficulty}&type={question_type}&token={token}"
    response = requests.get(url)
    if response.status_code == 200:
        return response.json()["results"]
    else:
        print("Failed to fetch questions.")
        return []

def create_database(conn):
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS QUESTIONS (
                 id INTEGER PRIMARY KEY,
                 question TEXT,
                 answers TEXT,
                 correct_answer_index INTEGER
                 )''')
    conn.commit()

def insert_questions(conn, questions):
    c = conn.cursor()
    for question in questions:
        answers = question["incorrect_answers"] + [question["correct_answer"]]
        correct_answer_index = len(answers) - 1
        c.execute('''INSERT INTO QUESTIONS (question, answers, correct_answer_index)
                     VALUES (?, ?, ?)''', (question["question"], ",".join(answers), correct_answer_index))
    conn.commit()

def main():
    try:
        conn = sqlite3.connect("triviaDB.sqlite")
        create_database(conn)
        
        token_response = requests.get("https://opentdb.com/api_token.php?command=request")
        if token_response.status_code == 200:
            token = token_response.json().get("token")
            questions = fetch_questions(token=token)
            insert_questions(conn, questions)
            conn.close()
            print("Questions inserted successfully.")
        else:
            print("Failed to retrieve token for fetching questions.")
            
    except sqlite3.Error as e:
        print("SQLite error:", e)

if __name__ == "__main__":
    main()
