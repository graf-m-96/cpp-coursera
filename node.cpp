#include "node.h"
#include "date.h"

#include <tuple>

using namespace std;

bool EmptyNode::Evaluate(const Date& variable_date, const string& variable_event) const {
    return true;
}

DateComparisonNode::DateComparisonNode(const Comparison cmp, const Date& date) : _cmp(cmp), _date(date) {}

bool DateComparisonNode::Evaluate(const Date& variable_date, const string& variable_event) const {
    const auto variable_tuple = tie(variable_date.Year, variable_date.Month, variable_date.Day);
    const auto other_tuple = tie(_date.Year, _date.Month, _date.Day);
    return compare(variable_tuple, other_tuple, _cmp);
}

EventComparisonNode::EventComparisonNode(const Comparison cmp, const string& event) : _cmp(cmp), _event(event) {}

bool EventComparisonNode::Evaluate(const Date& variable_date, const string& variable_event) const {
    return compare(variable_event, _event, _cmp);
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation logical_operation, shared_ptr<Node> left, const shared_ptr<Node> right)
    :  _logical_operation(logical_operation), _left(left), _right(right) {}

bool LogicalOperationNode::Evaluate(const Date& variable_date, const string& variable_event) const {
    if (_logical_operation  == LogicalOperation::And) {
        return _left->Evaluate(variable_date, variable_event) && _right->Evaluate(variable_date, variable_event);
    }

    if  (_logical_operation == LogicalOperation::Or) {
        return _left->Evaluate(variable_date, variable_event) || _right->Evaluate(variable_date, variable_event);
    }

    throw logic_error("unknown logical-operation");
}
