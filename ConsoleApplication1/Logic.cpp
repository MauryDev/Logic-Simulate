#include "Logic.h"
#include <functional>


LogicKlass Input::klass(static_cast<LogicKlass::OnUpdate>(&Input::OnUpdate), static_cast<LogicKlass::OnSinal>(&Input::OnSinal),LogicType::Input);
LogicKlass And::klass(static_cast<LogicKlass::OnUpdate>(&And::OnUpdate),static_cast<LogicKlass::OnSinal>(&And::OnSinal), LogicType::Input | LogicType::Output);
LogicKlass Or::klass(static_cast<LogicKlass::OnUpdate>(&Or::OnUpdate), static_cast<LogicKlass::OnSinal>(&Or::OnSinal), LogicType::Input | LogicType::Output);
LogicKlass Output::klass(static_cast<LogicKlass::OnUpdate>(&Output::OnUpdate), static_cast<LogicKlass::OnSinal>(&Output::OnSinal), LogicType::Output);
LogicKlass NAnd::klass(static_cast<LogicKlass::OnUpdate>(&NAnd::OnUpdate), static_cast<LogicKlass::OnSinal>(&NAnd::OnSinal), LogicType::Input | LogicType::Output);
LogicKlass Xor::klass(static_cast<LogicKlass::OnUpdate>(&Xor::OnUpdate), static_cast<LogicKlass::OnSinal>(&Xor::OnSinal), LogicType::Input | LogicType::Output);
LogicKlass Not::klass(static_cast<LogicKlass::OnUpdate>(&Not::OnUpdate), static_cast<LogicKlass::OnSinal>(&Not::OnSinal), LogicType::Output);
LogicKlass NOr::klass(static_cast<LogicKlass::OnUpdate>(&NOr::OnUpdate), static_cast<LogicKlass::OnSinal>(&NOr::OnSinal), LogicType::Input | LogicType::Output);
LogicKlass XNOr::klass(static_cast<LogicKlass::OnUpdate>(&XNOr::OnUpdate), static_cast<LogicKlass::OnSinal>(&XNOr::OnSinal), LogicType::Input | LogicType::Output);


bool LogicBase::IsInput()
{
    auto type = GetKlass().logicType;
    return (type & LogicType::Input) == LogicType::Input;
}

bool LogicBase::IsOnlyInput()
{
    auto type = GetKlass().logicType;

    return type == LogicType::Input;
}

bool LogicBase::isOutput()
{
    auto type = GetKlass().logicType;
    return (type & LogicType::Output) == LogicType::Output;
}

bool LogicBase::IsOnlyOutput()
{
    auto type = GetKlass().logicType;

    return type == LogicType::Output;
}

bool LogicBase::FirstInput()
{
    return Restant == input.size();
}


LogicKlass LogicBase::GetKlass()
{
    return LogicKlass();
}


void And::OnUpdate()
{
    this->Restant = this->input.size();
    this->Value = true;
}

void And::OnSinal(bool hot)
{
    this->Value = this->Value && hot;
    (this->Restant)--;

    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }

}

LogicKlass And::GetKlass()
{
    return And::klass;
}

void Or::OnUpdate()
{
    this->Restant = this->input.size();
}

void Or::OnSinal(bool hot)
{
    this->Value = this->Value || hot;
    (this->Restant)--;
    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }
}

LogicKlass Or::GetKlass()
{
    return Or::klass;
}

int Logic::AddAnd()
{
    auto id = currentid++;
    auto and_ = new And();
    and_->logicParent = this;
    this->logics[id] = and_;

    return id;
}

int Logic::AddNAnd()
{
    auto id = currentid++;
    auto and_ = new NAnd();
    and_->logicParent = this;
    this->logics[id] = and_;

    return id;
}

int Logic::AddOr()
{
    auto id = currentid++;
    auto or_ = new Or();
    or_->logicParent = this;
    this->logics[id] = or_;

    return id;
}

int Logic::AddXor()
{
    auto id = currentid++;
    auto or_ = new Xor();
    or_->logicParent = this;
    this->logics[id] = or_;

    return id;
}

int Logic::AddInput(bool v)
{
    auto id = currentid++;
    auto input_ = new Input();
    input_->logicParent = this;
    input_->Value = v;
    this->logics[id] = input_;

    return id;
}

int Logic::AddOutput()
{
    auto id = currentid++;
    auto input_ = new Output();
    input_->logicParent = this;
    this->logics[id] = input_;
    return id;
}

int Logic::AddNot()
{
    auto id = currentid++;
    auto input_ = new Not();
    input_->logicParent = this;
    this->logics[id] = input_;
    return id;
}

int Logic::AddNor()
{
    auto id = currentid++;
    auto input_ = new NOr();
    input_->logicParent = this;
    this->logics[id] = input_;
    return id;
}

int Logic::AddXnor()
{
    auto id = currentid++;
    auto input_ = new XNOr();
    input_->logicParent = this;
    this->logics[id] = input_;
    return id;
}

void Logic::AddES(int inputid, int outputid)
{
    auto input = Get(inputid);
    auto output = Get(outputid);

    if (input->IsInput())
    {
        if (output->isOutput())
        {
            input->output.push_back(outputid);
            output->input.push_back(inputid);
        }
    }
    
}

LogicBase* Logic::Get(int id)
{
    return logics[id];
}

void Logic::Run()
{
    for (auto& current : logics)
    {
        auto second = current.second;

        auto onsinal = second->GetKlass().onUpdate;
        (second->*onsinal)();
    }
    for (auto& current : logics)
    {
        auto second = current.second;

        if (second->IsOnlyInput())
        {
            auto onsinal = second->GetKlass().onSinal;
            (second->*onsinal)(true);
        }
    }
}

void Input::OnSinal(bool hot)
{
    
    if (this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }
}

void Input::OnUpdate()
{
    this->Restant = 0;
}


LogicKlass Input::GetKlass()
{
    return Input::klass;
}

LogicKlass::LogicKlass()
{
    this->logicType = LogicType::None;
    this->onSinal = NULL;
    this->onUpdate = NULL;
}

LogicKlass::LogicKlass(OnUpdate onupdate, OnSinal onsinal, LogicType ontype)
{
    this->logicType = ontype;
    this->onSinal = onsinal;
    this->onUpdate = onupdate;
}

void Output::OnSinal(bool hot)
{
    this->Value = hot;
    (this->Restant)--;
}

void Output::OnUpdate()
{
    this->Restant = this->input.size();
}

LogicKlass Output::GetKlass()
{

    
    return Output::klass;
}

void NAnd::OnUpdate()
{
    this->Value = false;
    this->Restant = this->input.size();
}

void NAnd::OnSinal(bool hot)
{
    this->Value = this->Value || !hot;
    (this->Restant)--;

    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }
}

LogicKlass NAnd::GetKlass()
{
    return NAnd::klass;
}

void Xor::OnUpdate()
{
    this->Restant = this->input.size();
    this->Value = false;
}

void Xor::OnSinal(bool hot)
{
    this->Value = this->Value != hot;
    (this->Restant)--;
    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
        
    }
}

LogicKlass Xor::GetKlass()
{
    return Xor::klass;
}

void Not::OnUpdate()
{
    this->Restant = this->input.size();
}

void Not::OnSinal(bool hot)
{
    this->Value = !hot;
    --(this->Restant);
    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }
}

LogicKlass Not::GetKlass()
{
    return Not::klass;
}

void NOr::OnUpdate()
{
    this->Restant = this->input.size();
    this->Value = true;
}

void NOr::OnSinal(bool hot)
{
    this->Value = this->Value && !hot;
    (this->Restant)--;
    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }
    }

}

LogicKlass NOr::GetKlass()
{
    return NOr::klass;
}

void XNOr::OnUpdate()
{
    this->Restant = input.size();
    
}

void XNOr::OnSinal(bool hot)
{
    this->Value = (this->FirstInput() ? hot :  this->Value == hot);
    (this->Restant)--;
    if (this->Restant == 0 && this->output.size() > 0)
    {
        for (auto c : this->output)
        {
            auto current = this->logicParent->Get(c);
            if (current->Restant > 0)
            {
                auto onsinal = current->GetKlass().onSinal;
                (current->*onsinal)(this->Value);
            }
        }

    }

}

LogicKlass XNOr::GetKlass()
{
    return XNOr::klass;
}
