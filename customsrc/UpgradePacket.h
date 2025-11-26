#pragma once

class UpgradePacket
{
	//Instanced for player classes;
private:
	int currency_ = 0;
	const int MAX_CURRENCY = 2147483647;


public:
	void PayCurrency(int pay_value)
	{
		currency_ -= pay_value;
	}

	int GetCurrency()
	{
		return currency_;
	}

	void AddCurrency(int added_value)
	{
		if (currency_ <= MAX_CURRENCY) { currency_ += added_value; }
	}

	float UpgradeAdd(float current_value, int added_value, int upgrade_cost)
	{
		if (InsufficientCurrency(upgrade_cost)) { DisplayFailUpgrade(); return 0; }
		PayCurrency(upgrade_cost);
		current_value += added_value;
		return current_value;
	}

	float UpgradeMult(float current_value, int added_value, int upgrade_cost)
	{
		if (InsufficientCurrency(upgrade_cost)) { DisplayFailUpgrade(); return 0; }
		PayCurrency(upgrade_cost);
		current_value *= added_value;
		return current_value;
	}

	bool InsufficientCurrency(int upgrade_cost)
	{
		if (currency_ - upgrade_cost < 0)
			return true;
	}

	void DisplayFailUpgrade()
	{
		//message
	}

};

UpgradePacket upgrade;