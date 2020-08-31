#include <stdio.h>
#include "mysql.h"
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "libmysql.lib")

const char *host = "localhost";
const char *user = "root";
const char *pw = "1234";
const char *db = "test";

MYSQL *connection = NULL;
MYSQL conn;
MYSQL_RES *sql_result;
MYSQL_ROW sql_row;

FILE *fp2;

void type1_1(char *truck_num)
{

    char query[1001];
    int state = 0;

    const char *tmp1;
    tmp1 = "select customer_name from customer, package, location, delivers where customer.ID = package.customer_ID and package.package_ID = delivers.package_ID  and location.package_ID = package.package_ID and location.tracking_date = '0625' and location.tracking_time = '1500' and location.status = 'on_delivering' and delivers.transportation_ID =";
    strcpy(query, tmp1);
    strncat(query, truck_num, sizeof(truck_num));
    state = mysql_query(connection, query);
    printf("**  Find all customers who had a package on the truck at the time of the crash. \n");
    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%s\n", sql_row[0]);
        }
    }
    printf("\n\n");
    mysql_free_result(sql_result);

    return;
}

void type1_2(char *truck_num)
{
    const char *tmp1;
    char query[1001];
    int state = 0;
    tmp1 = "select recipient.recipient_name, recipient_address from recipient, package, location, delivers where recipient.recipient_name = package.recipient_name and recipient.address = package.recipient_address and package.package_ID = delivers.package_ID  and location.package_ID = package.package_ID and location.tracking_date = '0625' and location.tracking_time = '1500' and location.status = 'on_delivering' and delivers.transportation_ID =";

    strcpy(query, tmp1);
    strncat(query, truck_num, sizeof(truck_num));
    state = mysql_query(connection, query);
    printf("**   Find all recipients who had a package on that truck at the time of the crash. \n");
    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%-10s %-10s\n", sql_row[0], sql_row[1]);
        }
    }
    mysql_free_result(sql_result);
    printf("\n\n");
}
void type1_3(char *truck_num)
{
    const char *tmp1, *tmp2, *tmp3;
    char query[1001];
    int state = 0;
    tmp1 = "select package.*, location.tracking_date from package, location where package.package_ID = location.package_ID and location.tracking_date =(select max(location.tracking_date) from package, delivers, location where package.package_ID = location.package_ID and location.status = 'complete' and delivers.package_ID = package.package_ID and delivers.transportation_ID =";
    strcpy(query, tmp1);
    strncat(query, truck_num, sizeof(truck_num));
    strcat(query, ") ");

    tmp2 = "and location. tracking_time = (select max(location.tracking_time) from package, delivers, location where package.package_ID = location.package_ID and location.status = 'complete' and delivers.package_ID = package.package_ID and delivers.transportation_ID =";
    strcat(query, tmp2);
    strncat(query, truck_num, sizeof(truck_num));
    strcat(query, ") ");

    tmp3 = "and location. tracking_year = (select max(location.tracking_year) from package, delivers, location where package.package_ID = location.package_ID and location.status = 'complete' and delivers.package_ID = package.package_ID and delivers.transportation_ID =";
    strcat(query, tmp3);
    strncat(query, truck_num, sizeof(truck_num));
    strcat(query, ") ");

    state = mysql_query(connection, query);
    printf("**   Find the last successful delivery by that truck prior to the crash.  \n");
    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%-10s %-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
        }
    }
    mysql_free_result(sql_result);
    printf("\n\n");
}
void type2(char *year)
{
    const char *tmp1, *tmp2, *tmp3;
    char query[1001];
    int state = 0;
    tmp1 = "select customer_name from bill, customer where customer.ID = bill.customer_ID and pay_year =";
    strcpy(query, tmp1);
    strncat(query, year, sizeof(year));
    strcat(query, " ");
    tmp2 = "group by customer_ID having count(*) = (select max(cnt) from(select customer_ID, count(customer_ID) as cnt from bill where pay_year =";
    strcat(query, tmp2);
    strncat(query, year, sizeof(year));
    tmp3 = " group by customer_ID) as result)";
    strcat(query, tmp3);

    state = mysql_query(connection, query);

    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%-10s\n", sql_row[0]);
        }
    }
    mysql_free_result(sql_result);
}
void type3(char *year)
{
    const char *tmp1, *tmp2, *tmp3;
    char query[1001];
    int state = 0;
    tmp1 = "select customer_name,  sum(amount) from bill, customer where customer.ID = bill.customer_ID and pay_year =";
    strcpy(query, tmp1);
    strncat(query, year, sizeof(year));
    strcat(query, " ");
    tmp2 = "group by customer_ID having sum(amount)  = (select max(amount_sum) from (select customer_ID, sum(amount) as amount_sum from bill where pay_year =";
    strcat(query, tmp2);
    strncat(query, year, sizeof(year));
    tmp3 = " group by customer_ID) as result)";
    strcat(query, tmp3);

    state = mysql_query(connection, query);

    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%-10s %-10s\n", sql_row[0], sql_row[1]);
        }
    }
    mysql_free_result(sql_result);
}
void type4()
{
    const char *tmp1;
    char query[1001];
    int state = 0;
    tmp1 = "select distinct package.package_ID from package, location where package.package_ID = location.package_ID  and location.tracking_date >= package.scheduled_time";
    strcpy(query, tmp1);

    state = mysql_query(connection, query);

    if (state == 0)
    {
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            printf("%-10s\n", sql_row[0]);
        }
    }
    mysql_free_result(sql_result);
}
void type5(char *year, char *month, char *name)
{
    char *tmp0, *tmp1, *tmp2, *tmp3;
    char query[1001];
    char x[5];
    int state = 0;
    int tmp_month = 0;

    tmp1 = "select customer.customer_name, address, sum(amount) from bill, customer where bill.customer_ID = customer.ID and customer.customer_name = '";
    strcpy(query, tmp1);
    strncat(query, name, sizeof(name));
    strcat(query, "' and pay_year = ");

    strncat(query, year, sizeof(year));
    strcat(query, " ");
    tmp2 = "and ";
    strcat(query, tmp2);
    strncat(query, month, sizeof(month));
    strcat(query, "00");
    tmp3 = " <= pay_date and pay_date < ";
    strcat(query, tmp3);
    tmp_month = atoi(month) + 1;
    sprintf(x, "%d", tmp_month);
    strcat(query, x);
    strcat(query, "00");
    strcat(query, " group by customer_name, address");

    state = mysql_query(connection, query);

    if (state == 0)
    {
        fprintf(fp2, "-----A simple bill------\n");
        fprintf(fp2, "name      address   amount     \n");
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            fprintf(fp2, "%-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2]);
        }
    }
    mysql_free_result(sql_result);
    fprintf(fp2, "\n\n");

    //------------------------
    //----------------bill type 2--------------
    tmp1 = "select package_type, package.package_ID, customer.customer_name , amount, pay_year, pay_date, pay_time from bill, customer, package where bill.customer_ID = customer.ID and package.customer_ID = customer.ID and package.package_ID = bill.package_ID and customer.customer_name = '";
    strcpy(query, tmp1);
    strncat(query, name, sizeof(name));
    strcat(query, "' and pay_year = ");

    strncat(query, year, sizeof(year));
    strcat(query, " ");
    tmp2 = "and ";
    strcat(query, tmp2);
    strncat(query, month, sizeof(month));
    strcat(query, "00");
    tmp3 = " <= pay_date and pay_date < ";
    strcat(query, tmp3);
    tmp_month = atoi(month) + 1;
    sprintf(x, "%d", tmp_month);
    strcat(query, x);
    strcat(query, "00");
    strcat(query, " group by package_type, package.package_ID");

    state = mysql_query(connection, query);

    if (state == 0)
    {
        fprintf(fp2, "----- A bill listing charges by type of service------\n");
        fprintf(fp2, "type       package_ID name     amount     year      date      \n");
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            fprintf(fp2, "%-10s %-10s %-10s %-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
        }
    }
    mysql_free_result(sql_result);
    fprintf(fp2, "\n\n");

    //--------------------------------------
    //-----------------------bill type3
    tmp1 = "select package.package_ID, amount, pay_type, timeliness from bill, customer, package where bill.customer_ID = customer.ID and package.customer_ID = customer.ID and package.package_ID = bill.package_ID and customer.customer_name = '";
    strcpy(query, tmp1);
    strncat(query, name, sizeof(name));
    strcat(query, "' and pay_year = ");

    strncat(query, year, sizeof(year));
    strcat(query, " ");
    tmp2 = "and ";
    strcat(query, tmp2);
    strncat(query, month, sizeof(month));
    strcat(query, "00");
    tmp3 = " <= pay_date and pay_date < ";
    strcat(query, tmp3);
    tmp_month = atoi(month) + 1;
    sprintf(x, "%d", tmp_month);
    strcat(query, x);
    strcat(query, "00");
    strcat(query, " group by package.package_ID ,package_type");

    state = mysql_query(connection, query);

    if (state == 0)
    {
        fprintf(fp2, "-----  An itemized billing listing------\n");
        fprintf(fp2, "package_ID amount     pay_type  timeliness\n");
        sql_result = mysql_store_result(connection);
        while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
        {
            fprintf(fp2, "%-12s %-12s %-12s %-12s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
        }
    }
    mysql_free_result(sql_result);
    fprintf(fp2, "\n\n");
    fclose(fp2);
}

int main(void)
{

    if (mysql_init(&conn) == NULL)
        printf("mysql_init() error!");

    connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char *)NULL, 0);
    if (connection == NULL)
    {
        printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }

    else
    {
        printf("Connection Succeed\n");

        if (mysql_select_db(&conn, db))
        {
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        FILE *fp;
        char *query = (char *)malloc(1001);
        fp = fopen("20150411_1.txt", "r"); //create_table

        if (fp == NULL)
        {
            printf("create_table_txt file open error!!!\n");
            return 0;
        }
        char s[1001];
        int state = 0;
        while (fgets(s, 1001, fp) != NULL)
        {
            strcpy(query, s);

            state = mysql_query(connection, query); //execute query
            if (state != 0)
            {
                printf("create table failed!!!\n");
            }
        }
        fclose(fp);

        fp = fopen("20150411_2.txt", "r"); //insert_data

        if (fp == NULL)
        {
            printf("drop_table text file open error!!!\n");
            return 0;
        }

        while (fgets(s, 1001, fp) != NULL)
        {
            strcpy(query, s);
            state = mysql_query(connection, query); //execute query
            if (state != 0)
            {
                printf("insert failed!!!\n");
            }
        }

        int type = 1;
        int sub_type = 1;
        char year[20];
        char month[5];
        char name[20];
        printf("******************************\n");
        printf("welcome to sogang package delivery system\n");
        printf("******************************\n");

        while (1)
        {
            printf("\n\n");
            printf("----- select query type -----\n");
            printf("\t\t1. TYPE 1\n");
            printf("\t\t2. TYPE 2\n");
            printf("\t\t3. TYPE 3\n");
            printf("\t\t4. TYPE 4\n");
            printf("\t\t5. TYPE 5\n");
            printf("\t\t0. QUIT \n");

            printf("Which type of query? :");
            scanf("%d", &type);
            if (type == 0)
                break;
            switch (type)
            {
            case 1:
                while (1)
                {

                    char truck_num[20];

                    printf("---- TYPE 1 ----\n");
                    printf("Input the number of truck: ");
                    scanf("%s", truck_num);

                    if (strcmp(truck_num, "0") == 0)
                    { //if input is 0, break
                        break;
                    }

                    char *query = (char *)malloc(1001);
                    int state = 0;

                    const char *tmp = "select distinct transportation_ID from delivers where transportation_ID =";
                    strcpy(query, tmp);
                    strncat(query, truck_num, sizeof(truck_num));

                    state = mysql_query(connection, query);
                    sql_result = mysql_store_result(connection);

                    if (mysql_fetch_row(sql_result) == NULL)
                    {
                        printf("%s truck does not exist!\n", truck_num);
                        mysql_free_result(sql_result);
                    }
                    else
                    {
                        printf("\n\n--- subtypes in TYPE 1 ---\n");
                        printf("\t\t 1. TYPE 1-1\n");
                        printf("\t\t 2. TYPE 1-2\n");
                        printf("\t\t 3. TYPE 1-3\n");
                        printf("\t\t 0. BACK TO PREVIOUS MENU\n\n");

                        printf("Which type of sub_query? :");
                        scanf("%d", &sub_type);
                        if (sub_type == 0)
                        {
                            break;
                        }
                        mysql_free_result(sql_result);

                        switch (sub_type)
                        {
                        case 1:
                            type1_1(truck_num);
                            break;
                        case 2:
                            type1_2(truck_num);
                            break;
                        case 3:
                            type1_3(truck_num);
                            break;
                        }
                    }
                }
                break;
            case 2:

                printf("---- TYPE 2 ----\n");
                printf("**  Find the customer who has shipped the most packages in the past certain year\n");
                printf("Which year? : ");
                scanf("%s", year);
                type2(year);

                break;
            case 3:
                printf("---- TYPE 3 ----\n");
                printf("**  Find the customer who has shipped the most packages in the past certain year\n");
                printf("Which year? : ");
                scanf("%s", year);
                type3(year);
                break;
            case 4:
                printf("---- TYPE 4 ----\n");
                printf("**   Find those packages that were not delivered within the promised time.\n");
                type4();
                break;
            case 5:
                fp2 = fopen("bill.txt", "w");
                printf("---- TYPE 5 ----\n");
                printf("**    Generate the bill for each customer for the past certain month\n");
                printf("customer name?: ");
                scanf("%s", name);
                printf("Which year?: ");
                scanf("%s", year);
                printf("Which month?: ");
                scanf("%s", month);
                printf("**    Generating the bill....\n");
                type5(year, month, name);
                printf("**    Generation completed!!!\n");
                break;
            }
        }
        //------------drop table---------------

        fp = fopen("20150411_3.txt", "r"); //drop table

        if (fp == NULL)
        {
            printf("drop_table text file open error!!!\n");
            return 0;
        }

        while (fgets(s, 1001, fp) != NULL)
        {
            strcpy(query, s);

            state = mysql_query(connection, query); //execute query
            if (state != 0)
            {
                printf("drop table failed!!!\n");
            }
        }

        mysql_close(connection);
        free(query);

        fclose(fp);
    }

    return 0;
}
