import mysql.connector
from mysql.connector import errorcode
import pymysql
import pandas as pd
import numpy as np
from sqlalchemy import create_engine
import keras
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt

def flatten(col_to_extract, index_col, df):
    extracted = df[col_to_extract].astype(str)
    extracted=extracted.apply(lambda x: x.split(","))
    index = df[index_col]
    new_df= pd.concat([index, extracted], axis=1)
    new_df = new_df.explode(col_to_extract)
    return new_df


def plot(history):  # to plot graphs used in report
    plt.plot(history.history["accuracy"])
    plt.plot(history.history["val_accuracy"])
    plt.title("Testing v Training Accuracy {}".format(self.sizes))
    plt.ylabel("Accuracy")
    plt.xlabel("Epoch")
    plt.legend(["training", "validation"], loc="upper left")
    plt.show()

if __name__ == '__main__':
    IP = "192.168.0.57"
    cnx = mysql.connector.connect(user="hammad", password="hammadtrishal", host=IP, database="ece656project", auth_plugin="mysql_native_password")
    cursor = cnx.cursor()
    #query = "select nconst, ratings.tconst as tconst, averageRating from actorTitles inner join ratings on actorTitles.tconst = ratings.tconst"
    query = "select nconst, tconst from actorTitles"
    df = pd.read_sql(query, con=cnx)
    #df["nconst"] = df["nconst"].apply(lambda x: x.lstrip("nm"))
    #df["tconst"] = df["tconst"].apply(lambda x: x.lstrip("tt"))
    df = df.sample(20000)
    df.dropna(inplace=True)
    #df.set_index("tconst", inplace=True)
    #y = df["averageRating"]
    #df.drop(["averageRating"], axis=1, inplace=True)
    #df = df.astype(np.float64)
    #dfSmall = df.samples(100000)
    #unique = df.pivot_table(values="averageRating", columns="nconst", index="tconst")
    unique = pd.crosstab(df.tconst, df.nconst)
    tempy = {}
    for row in unique.iterrows():
        tconst = row[0]
        query = f"select averageRating from ratings where tconst = \"{tconst}\""
        cursor.execute(query)
        for (averageRating) in cursor:
            tempy[tconst]=averageRating[0]
    y = pd.Series(tempy)
    X = unique[unique.index.isin(y.index)]


    '''new_df = flatten("knownForTitles", "nconst", df)
    engine = create_engine("mysql://hammad:hammadtrishal@192.168.0.57/ece656project")
    con = engine.connect()
    new_df.to_sql("actorTitles", con=con, if_exists="append", index=False, chunksize=10000)'''
    #X = df[["nconst", "tconst"]]
    #y = df["averageRating"]
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.33)
    model = keras.Sequential()
    model.add(keras.layers.Dense(X.shape[1], activation="sigmoid"))
    model.add(keras.layers.Dense(3000, activation="sigmoid"))
    model.add(keras.layers.Dense(2000, activation="sigmoid"))
    model.add(keras.layers.Dense(1000, activation="sigmoid"))
    model.add(keras.layers.Dense(512, activation="sigmoid"))
    model.add(keras.layers.Dense(1, activation="sigmoid"))
    opt = keras.optimizers.Adam(learning_rate=0.00015)
    model.compile(optimizer=opt, loss="mean_squared_error", metrics=["accuracy"])
    fitted = model.fit(X_train, y_train, epochs=1000, batch_size=64,
                            validation_data=(X_test, y_test))
    #plot(fitted)




