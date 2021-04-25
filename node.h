#pragma once

#include "date.h"

#include <string>
#include <memory>

using namespace std;

enum Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum LogicalOperation {
    Or,
    And
};

class Node {
public:
    virtual bool Evaluate(const Date& variable_date, const string& variable_event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date& variable_date, const string& variable_event) const override;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison cmp, const Date& _date);

    bool Evaluate(const Date& variable_date, const string& variable_event) const override;

private:
    const Comparison _cmp;
    const Date _date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison cmp, const string& _event);

    bool Evaluate(const Date& variable_date, const string& variable_event) const override;

private:
    const Comparison _cmp;
    const string _event;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation logical_operation, shared_ptr<Node> left, const shared_ptr<Node> right);

    bool Evaluate(const Date& variable_date, const string& variable_event) const override;

private:
    const LogicalOperation _logical_operation;
    const shared_ptr<Node> _left;
    const shared_ptr<Node> _right;
};

template <class T>
bool compare(const T& a, const T& b, Comparison cmp) {
    if (cmp == Comparison::Equal) {
        return a == b;
    }

    if (cmp == Comparison::Greater) {
        return a > b;
    }

    if (cmp == Comparison::GreaterOrEqual) {
        return a >= b;
    }

    if (cmp == Comparison::Less) {
        return a < b;
    }

    if (cmp == Comparison::LessOrEqual) {
        return a <= b;
    }

    if (cmp == Comparison::NotEqual) {
        return a != b;
    }

    throw logic_error("unknown comparison");
}