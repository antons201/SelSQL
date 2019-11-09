
#include <gtest/gtest.h>
#include "../Client/Client.h"
#include "../src/Utils/Structures/Data/Variable.h"
#include "Headers/TestUtils.h"
#include "parser.cpp"

TEST(SERVER_TEST_CREATE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bb(id INT PRIMARY KEY UNIQUE NOT NULL, age float NOT NULL UNIQUE, name "
                               "char(50));",
                               "Success"},
                              {"INSERT INTO bb values(1, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bb;",
                               "CREATE TABLE bb(id INT PRIMARY KEY UNIQUE NOT NULL, age FLOAT NOT NULL UNIQUE, name "
                               "CHAR(50));"},
                              {"SELECT * from bb;",
                               "\nid|age      |name   |\n"
                               "1 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_CREATE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bbb(id INT PRIMARY KEY, age float NOT NULL, name char(50) UNIQUE);",
                               "Success"},
                              {"INSERT INTO bbb values(0, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bbb;",
                               "CREATE TABLE bbb(id INT PRIMARY KEY, age FLOAT NOT NULL, name CHAR(50) UNIQUE);"},
                              {"SELECT * from bbb;",
                               "\nid|age      |name   |\n"
                               "0 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_CREATE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bbbb(id INT, age float, name char(30));", "Success"},
                              {"INSERT INTO bbbb values(0, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bbbb;", "CREATE TABLE bbbb(id INT, age FLOAT, name CHAR(30));"},
                              {"SELECT * from bbbb;",
                               "\nid|age      |name   |\n"
                               "0 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_SELECT, TEST1) {
//    TestUtils::StartServer();
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE t(id INT PRIMARY KEY);", "Success"},
                              {"INSERT INTO t values(0);", "Success"},
                              {"INSERT INTO t values(1);", "Success"},
                              {"SELECT * from t;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE b(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO b values(0, 10);", "Success"},
                              {"INSERT INTO b values(1, 20);", "Success"},
                              {"SELECT * from b;",
                               "\nid|age|\n"
                               "0 |10 |\n"
                               "1 |20 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bz(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO bz values(0, 10);", "Success"},
                              {"INSERT INTO bz values(1, 20);", "Success"},
                              {"SELECT id from bz;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bx(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bx values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bx values(1, 20, 'tyty');", "Success"},
                              {"SELECT id, age, name from bx;",
                               "\nid|age|name  |\n"
                               "0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_SELECT, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bc(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bc values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bc values(1, 20, 'tyty');", "Success"},
                              {"SELECT *, id, age, name, * from bc;",
                               "\nid|age|name  |id|age|name  |id|age|name  |\n"
                               "0 |10 |'sdf' |0 |10 |'sdf' |0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|1 |20 |'tyty'|1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qwe(id CHAR(50));", "Success"},
                              {"insert into qwe values (1);", "Success"},
                              {"select * from qwe;",
                               "\nid |\n"
                               "'1'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qd(id int);", "Success"},
                              {"insert into qd values (22.22);", "Success"},
                              {"select * from qd;",
                               "\nid|\n"
                               "22|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values (2.2);", "Success"},
                              {"select * from qh;",
                               "\nid        |\n"
                               "'2.200000'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int);", "Success"},
                              {"insert into qh values (1);", "Success"},
                              {"select * from qh;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_INSERT, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values ('dgdf dfg %');", "Success"},
                              {"select * from qh;",
                               "\nid          |\n"
                               "'dgdf dfg %'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id float);", "Success"},
                              {"insert into qh values (55.55);", "Success"},
                              {"select * from qh;",
                               "\nid       |\n"
                               "55.550000|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(50));", "Success"},
                              {"insert into qh values ('ssdf 77 & % sdfsdf*/sdf''s');", "Success"},
                              {"select * from qh;",
                               "\nid                         |\n"
                               "'ssdf 77 & % sdfsdf*/sdf's'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(id) values (6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name|\n"
                               "6663|    |\n"}});
}

TEST(SERVER_TEST_INSERT, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(name, id) values ('sdf', 6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name |\n"
                               "6663|'sdf'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh values (666, 'sfd');", "Success"},
                              {"select * from qh;",
                               "\nid |name |\n"
                               "666|'sfd'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST11) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE);", "Success"},
                              {"insert into qh values (-5);", "Success"},
                              {"select * from qh;",
                               "\nid|\n"
                               "-5|\n"}});
}

// TEST(SERVER_TEST_INSERT, TEST12) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE qh(id int UNIQUE, val int, age float, name char(10));", "Success"},
//                              {"insert into qh(val, name) values (3, 'sdf');", "Success"},
//                              {"select * from qh;",
//                               "\nid|val|age|name |\n"
//                               "  |3  |   |'sdf'|\n"}});
//}

TEST(SERVER_TEST_SHOW_CREATE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE g(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"SHOW CREATE TABLE g;", "CREATE TABLE g(id INT PRIMARY KEY, age INT NOT NULL);"}});
}

TEST(SERVER_TEST_SHOW_CREATE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE h(id INT);", "Success"},
                              {"SHOW CREATE TABLE h;", "CREATE TABLE h(id INT);"}});
}

TEST(SERVER_TEST_UPDATE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE c(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO c values(1, 20);", "Success"},
                              {"UPDATE c SET id = 8;", "Success"},
                              {"SELECT * from c;",
                               "\nid|age|\n"
                               "8 |20 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE d(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO d values(10, 20);", "Success"},
                              {"UPDATE d SET id = 15, age = 9;", "Success"},
                              {"SELECT * from d;",
                               "\nid|age|\n"
                               "15|9  |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE dd(age float PRIMARY KEY, name char(100));", "Success"},
                              {"INSERT INTO dd values(1.5, 'Petya');", "Success"},
                              {"UPDATE dd SET age = 5.5, name = 'Vasya';", "Success"},
                              {"SELECT * from dd;",
                               "\nage     |name   |\n"
                               "5.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n2 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 3 where id = 2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n"
                               "3 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = 1;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = 2+2*2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "6 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = -3*(2+2*2)+6*2.5;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "-3|\n"}});
}

TEST(SERVER_TEST_DELETE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE e(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"DELETE FROM e;", "Success"},
                              {"SELECT * from e;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE f(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO f values(10, 20);", "Success"},
                              {"INSERT INTO f values(1, 2);", "Success"},
                              {"DELETE FROM f;", "Success"},
                              {"SELECT * from f;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ff(id INT , age float NOT NULL, name char(75));", "Success"},
                              {"INSERT INTO ff values(10, 2.0, 'Vvv');", "Success"},
                              {"INSERT INTO ff values(1, 2.789, 'adsasdasdasdasd');", "Success"},
                              {"DELETE FROM ff;", "Success"},
                              {"SELECT * from ff;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fff(id INT , age float NOT NULL, name char(75));", "Success"},
                              {"INSERT INTO fff values(10, 2.0, 'Vvv');", "Success"},
                              {"INSERT INTO fff values(1, 2.789, 'adsasdasdasdasd');", "Success"},
                              {"DELETE FROM fff where id = 1;", "Success"},
                              {"SELECT * from fff;",
                               "\nid|age     |name |\n"
                               "10|2.000000|'Vvv'|\n"},
                              {"DELETE FROM fff where name = 'Vvv';", "Success"},
                              {"SELECT * from fff;", "Success"}});
}

TEST(SERVER_TEST_ALL, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fe(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO fe values(10, 20);", "Success"},
                              {"INSERT INTO fe values(1, 2);", "Success"},
                              {"SELECT * from fe;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE fe SET id = 5;", "Success"},
                              {"SELECT * from fe;",
                               "\nid|age|\n"
                               "5 |20 |\n"
                               "5 |2  |\n"}});
}

TEST(SERVER_TEST_WHERE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fr(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO fr values(10, 20);", "Success"},
                              {"INSERT INTO fr values(1, 2);", "Success"},
                              {"SELECT * from fr where id = 2;", "Success"},
                              {"SELECT * from fr where id = 1;",
                               "\nid|age|\n"
                               "1 |2  |\n"},
                              {"UPDATE fr SET id = 5 where id = 4;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE fr SET id = 5 where not id = 10;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "5 |2  |\n"},
                              {"DELETE FROM fr where id = 5;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"}});
}

TEST(SERVER_TEST_WHERE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ft(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO ft values(10, 20);", "Success"},
                              {"INSERT INTO ft values(1, 2);", "Success"},
                              {"SELECT * from ft where id = 4*(7-2) - 10;",
                               "\nid|age|\n"
                               "10|20 |\n"},
                              {"SELECT * from ft where id = 1 or id = 2;",
                               "\nid|age|\n"
                               "1 |2  |\n"},
                              {"UPDATE ft SET id = 5 where id = 2 and id = 1;", "Success"},
                              {"SELECT * from ft;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE ft SET id = 5 where not id = 10;", "Success"},
                              {"SELECT * from ft;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "5 |2  |\n"}});
}

TEST(SERVER_TEST_WHERE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fy(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fy values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fy values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fy values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fy where id + 1 = id + 1;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fy where id = 1 or id = 2;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"},
                              {"SELECT * from fy where id = 3 and name = 'sfsf' or name = 'qwerty';",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"},
                              {"SELECT * from fy where id = 3 or name = 'hgfdsa';",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fy where not age = 2.9 and name = 'qwerty';",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"}});
}

TEST(SERVER_TEST_WHERE, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fo(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fo values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fo values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fo values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fo where id + 5 = id + 1;", "Success"},
                              {"SELECT * from fo where not(id = (1+7)*2 - 13 or id = 2 and age = 3.789) and name = "
                               "'sfsf';",
                               "\nid|age     |name  |\n"
                               "1 |2.900000|'sfsf'|\n"},
                              {"SELECT * from fo where not id = 3 and (name = 'sfsf' or id = 1');",
                               "\nid|age     |name  |\n"
                               "1 |2.900000|'sfsf'|\n"},
                              {"SELECT * from fo where id = 3 or not name = 'hgfdsa' or age = '0.789';",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fo where id = 18/(2+2*2) and not(age = 2.9 and name = 'sfsf');",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"}});
}

TEST(SERVER_TEST_WHERE, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fp(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fp values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fp values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fp values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fp where id + 5 > id + 1;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id > 1;",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where name > 'a';",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id != 3 or not name != 'hgfdsa' and age <= 3.789;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id >= 18/(2+2*2) and age != 2.9;",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"}});
}

TEST(SERVER_TEST_WHERE, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn where id = name;", "Success"}});
}

TEST(SERVER_TEST_JOIN, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"SELECT * from jj join jg on jj.id = jg.id;",
                               "\njj.id|jj.age  |jj.name |jg.id|jg.age  |\n"
                               "1    |2.900000|'sfsf'  |1    |3.500000|\n"
                               "2    |3.789000|'qwerty'|2    |3.700000|\n"},
                              {"SELECT j1.id, j2.name from jj as j1 join jj as j2 on j1.id = j2.id;",
                               "\nj1.id|j2.name  |\n"
                               "1    |'sfsf'   |\n"
                               "2    |'qwerty' |\n"
                               "5    |'qwesdfy'|\n"}});
}

TEST(SERVER_TEST_JOIN, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE jh(id INT NOT NULL , some float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"INSERT INTO jh values(3, 9.5);", "Success"},
                              {"INSERT INTO jh values(1, 3.7);", "Success"},
                              {"INSERT INTO jh values(2, 3.789);", "Success"},
                              {"SELECT * from (jj as j1 join jg as j2 on j1.id = j2.id) join jh on j1.age = jh.some;",
                               "\njh.id|jh.some |j1.id|j1.age  |j1.name |j2.id|j2.age  |\n"
                               "2    |3.789000|2    |3.789000|'qwerty'|2    |3.700000|\n"},
                              {"SELECT * from jh as j3 join (jj as j1 join jg as j2 on j1.age = j2.age) on j3.id = "
                               "j2.id;",
                               "\nj3.id|j3.some |j1.id|j1.age  |j1.name  |j2.id|j2.age  |\n"
                               "3    |9.500000|1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "2    |3.789000|5    |3.700000|'qwesdfy'|2    |3.700000|\n"}});
}

TEST(SERVER_TEST_JOIN, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE jh(id INT NOT NULL , some float);", "Success"},
                              {"CREATE TABLE jk(id INT NOT NULL , some float, name char(150));", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"INSERT INTO jh values(3, 9.5);", "Success"},
                              {"INSERT INTO jh values(1, 3.7);", "Success"},
                              {"INSERT INTO jh values(2, 3.789);", "Success"},
                              {"INSERT INTO jk values(1, 3, 'sfsf');", "Success"},
                              {"SELECT * from (jj as j1 join jg as j2 on j1.age = j2.age) join (jk join jh on jk.id = "
                               "jh.id) on j1.name = jk.name;",
                               "\nj1.id|j1.age  |j1.name|j2.id|j2.age  |jh.id|jh.some |jk.id|jk.some |jk.name|\n"
                               "1    |2.900000|'sfsf' |3    |2.900000|1    |3.700000|1    |3.000000|'sfsf' |\n"},
                              {"SELECT * from ((jk join jg on jk.id = jg.id) join (jj as j1 join jg as j2 on j1.id = "
                               "j2.id) on jk.id = jg.id) join (jj as j3 join jg as j4 on j3.age = j4.age) on j1.id = "
                               "j4.id;",
                               "\njg.id|jg.age  |jk.id|jk.some |jk.name|j1.id|j1.age  |j1.name |j2.id|j2.age  "
                               "|j3.id|j3.age  |j3.name  |j4.id|j4.age  |\n"
                               "1    |3.500000|1    |3.000000|'sfsf' |2    |3.789000|'qwerty'|2    |3.700000|5    "
                               "|3.700000|'qwesdfy'|2    |3.700000|\n"}});
}

TEST(SERVER_TEST_JOIN, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jl(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jp(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jl values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jl values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jl values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jp values(1, 3.5);", "Success"},
                              {"INSERT INTO jp values(2, 3.7);", "Success"},
                              {"INSERT INTO jp values(3, 2.9);", "Success"},
                              {"SELECT * from jl join jp on jl.id = jl.id and jl.age = jp.age;",
                               "\njl.id|jl.age  |jl.name  |jp.id|jp.age  |\n"
                               "1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "5    |3.700000|'qwesdfy'|2    |3.700000|\n"},
                              {"SELECT j1.id, j2.age from jl as j1 join jp as j2 on j1.id = j2.id or j1.age = j2.age;",
                               "\nj1.id|j2.age  |\n"
                               "1    |3.500000|\n"
                               "1    |2.900000|\n"
                               "2    |3.700000|\n"
                               "5    |3.700000|\n"}});
}

TEST(SERVER_TEST_ERROR, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE i(id INT);", "Success"},
                              {"CREATE TABLE i(id INT);", "Table already exists ERROR: 1"}});
    TestUtils::clear();
}

TEST(SERVER_TEST_ERROR, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asa(id INT UNIQUE UNIQUE, id1 FLOAT NOT NULL);",
                               "Constraint already exists ERROR: 3"}});
}

TEST(SERVER_TEST_ERROR, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, id FLOAT NOT NULL);",
                               "Field name already used ERROR: 4"}});
}

TEST(SERVER_TEST_ERROR, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE m(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into m values(1, 2);", "Success"},
                              {"insert into m values(null, null);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE mm(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into mm values(1, 2);", "Success"},
                              {"insert into mm values(3, 4);", "Success"},
                              {"update mm set di = null, id = 3;", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into qqq values(1, 2);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into ac(id) values(1, 2);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE k(id INT UNIQUE);", "Success"},
                              {"insert into k values(1);", "Success"},
                              {"insert into k values(2);", "Success"},
                              {"insert into k values(1);", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values('sdfsdf');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT NOT NULL);", "Success"},
                              {"insert into asas(id) values(1);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST11) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into asas(isasfdasf, id) values(1, 2);", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST12) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"DROP TABLE asas;", "Success"},
                              {"insert into asas values (1, 5.5);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST13) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values(159753159753);",
                               "Int oversize (Str num 1, sym num 30): 159753159753"}});
}

TEST(SERVER_TEST_ERROR, TEST14) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qw(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into qw(id, id) values (1, 3);", "Same column in request ERROR: 7"}});
}

TEST(SERVER_TEST_ERROR, TEST15) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qe(id CHAR(10));", "Success"},
                              {"insert into qe values ('qwertyuiopqwertyuio');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, TEST16) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(256));", "Success"},
                              {"insert into qh values ('qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "q"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "q"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq');",
                               "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, TEST17) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(1));", "Success"},
                              {"insert into qh values ('');", "Success"},
                              {"insert into qh values ('a');", "Success"},
                              {"insert into qh values ('aa');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, TEST18) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, TEST19) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(age float);", "Success"},
                              {"INSERT INTO fn values('dfgdg');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, TEST20) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 1;",
                               "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST21) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 3;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST22) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST23) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique, name char(50));", "Success"},
                              {"insert into qg values (1, 'qwe');", "Success"},
                              {"insert into qg values (2, 'ewq');", "Success"},
                              {"select * from qg where id = name + 1;", "Types mismatch ERROR: 11"}});
}

// TEST(SERVER_TEST_ERROR, TEST24) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
//                              {"SELECT * from fn where qqq = poi;",
//                               "Table fn:\n"}});  // TODO проверять существование колонки
//}

TEST(SERVER_TEST_ERROR, TEST25) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join nf on fn.id = nf.id;", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST26) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn as f1 join fn on f1.id = fn.id11;", "Field doesnt exist ERROR: 6"}});
}

// TEST(SERVER_TEST_ERROR, TEST27) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
//                              {"SELECT * from fn join fn on fn.id = fn.id;",
//                               "Field doesnt exist ERROR: 6"}});  // TODO чекать на алиас в одной и той же таблице
//}

TEST(SERVER_TEST_ERROR, TEST28) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join fn on f1.id = fn.id;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST29) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"SELECT dgdfg, id from fn;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"с", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t ();",
                               "syntax error, unexpected RBRACKET, expecting IDENT (Str num 1, sym num 14): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"creat table t (id int);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): creat"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create tale t (id int);",
                               "syntax error, unexpected IDENT, expecting TABLE (Str num 1, sym num 10): tale"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table 555 (id int);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 14): 555"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id innt);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str num "
                               "1, sym num 19): innt"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id charrrr(50));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str num "
                               "1, sym num 22): charrrr"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char(45454554525452452452452));",
                               "syntax error, unexpected $undefined, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE "
                               "(Str num 1, sym num 46): char(45454554525452452452452)"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char54545(20));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str num "
                               "1, sym num 26): char54545"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST11) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name 456456);",
                               "syntax error, unexpected NUMBER, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num 1, sym num 23): 456456"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST12) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table 645 (name int);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 14): 645"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST13) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int, nn char);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str num "
                               "1, sym num 27): char"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST14) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int;",
                               "syntax error, unexpected SEMICOLON, expecting RBRACKET or COMMA (Str num 1, sym num "
                               "21): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST15) {
    TestUtils::clear();
    TestUtils::checkRequests({{"dro table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): dro"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST16) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t, y;",
                               "syntax error, unexpected COMMA, expecting SEMICOLON (Str num 1, sym num 11): ,"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST17) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t",
                               "syntax error, unexpected $end, expecting SEMICOLON (Str num 1, sym num 10): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST18) {
    TestUtils::clear();
    TestUtils::checkRequests({{"shw create table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): shw"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST19) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show crate table t;",
                               "syntax error, unexpected IDENT, expecting CREATE_ACTION (Str num 1, sym num 9): "
                               "crate"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST20) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show create table (t);",
                               "syntax error, unexpected LBRACKET, expecting IDENT (Str num 1, sym num 16): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST21) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into 'tt' values(1);",
                               "syntax error, unexpected STRVAL, expecting IDENT (Str num 1, sym num 14): 'tt'"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST22) {
    TestUtils::clear();
    TestUtils::checkRequests({{"inser into t values(1);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): inser"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST23) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert ito t values(1);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 9): ito"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST24) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(1);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 13): 1"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST25) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) valus;",
                               "syntax error, unexpected IDENT, expecting VALUES (Str num 1, sym num 21): valus"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST26) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) (5);",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST27) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t (col) (col);",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST28) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t (col) ();",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST29) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values;",
                               "syntax error, unexpected SEMICOLON, expecting LBRACKET (Str num 1, sym num 18): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST30) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values();",
                               "syntax error, unexpected RBRACKET (Str num 1, sym num 19): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST31) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert t values(7);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 7): t"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST32) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t join tt;",
                               "syntax error, unexpected SEMICOLON, expecting ON (Str num 1, sym num 19): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST33) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t jon tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting SEMICOLON (Str num 1, sym num 15): jon"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST34) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as join tt on t.id = tt.id;",
                               "syntax error, unexpected JOIN, expecting IDENT (Str num 1, sym num 18): join"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST35) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t a t1 join tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting SEMICOLON (Str num 1, sym num 13): a"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST36) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as t1 join tt t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting ON (Str num 1, sym num 23): t"}});
}

// TEST(SERVER_TEST_SYN_STRESS, TEST1) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
//                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
//                              //funChangeFile()
//                              {"select * from jj;", "Success"}});
//}
//
// TEST(SERVER_TEST_SYN_STRESS, TEST2) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
//                              //funChangeFile()
//                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
//                              //funChangeFile()
//                              //funChangeFile()
//                              {"select * from jj;", "Success"}});
//}