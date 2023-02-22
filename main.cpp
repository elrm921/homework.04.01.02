#include <iostream>
#include <vector>
#include <map>

class SqlSelectQueryBuilder
{
public:
    SqlSelectQueryBuilder() = default;

    SqlSelectQueryBuilder& AddColumn(const std::string &column)
    {
        columns.push_back(column);
        return *this;
    }
    SqlSelectQueryBuilder& AddFrom(const std::string &table)
    {
        tables.push_back(table);
        return *this;
    }
    SqlSelectQueryBuilder& AddWhere(const std::string &key, const std::string &value)
    {
        conditions.push_back(std::pair(key,value));
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept
    {
        for (auto pair : kv)
        {
            AddWhere(pair.first, pair.second);
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept
    {
        for (auto col : columns)
        {
            AddColumn(col);
        }
        return *this;
    }

    std::string BuildQuery()
    {
        int columns_size = columns.size();
        if (columns_size > 0) querry_string = "SELECT ";
        else querry_string = "SELECT * ";
        for (int i=0; i<columns_size; i++)
        {
            querry_string += columns[i];
            if (i < columns_size-1) querry_string += ", ";
        }

        int tables_size = tables.size();
        if (tables_size > 0) querry_string += " FROM ";
        for (int i=0; i<tables_size; i++)
        {
            querry_string += tables[i];
            if (i < tables_size-1) querry_string += ", ";
        }

        int conditions_size = conditions.size();
        if (conditions_size > 0) querry_string += " WHERE ";
        for (int i=0; i<conditions_size; i++)
        {
            querry_string += conditions[i].first + "=" + conditions[i].second;
            if (i < conditions_size-1) querry_string += " AND ";
        }
        querry_string += ';';

        return querry_string;
    }
private:
    std::vector<std::string> columns;
    std::vector<std::string> tables;
    std::vector<std::pair<std::string,std::string>> conditions;
    std::string querry_string;
};

int main()
{
    SqlSelectQueryBuilder query_builder;
    query_builder.AddFrom("students");
    std::vector<std::string> columns_vec = {"name", "phone"};
    query_builder.AddColumns(columns_vec);
    std::map<std::string, std::string> conditions_map = {{"id", "42"}, {"name", "John"}};
    query_builder.AddWhere(conditions_map);

    auto test = query_builder.BuildQuery();
    std::cout << test << std::endl;

    assert(test == "SELECT name, phone FROM students WHERE id=42 AND name=John;");
    
    return 0;
}