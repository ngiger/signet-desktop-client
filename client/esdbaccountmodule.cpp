#include "esdbaccountmodule.h"
#include "accountactionbar.h"

#include "esdb.h"
#include "account.h"

EsdbActionBar *esdbAccountModule::newActionBar()
{
	return new AccountActionBar(m_parent);
}

esdbEntry *esdbAccountModule::decodeEntry(int id, int revision, esdbEntry *prev, struct block *blk) const
{
	esdbEntry *entry = NULL;
	account *acct = NULL;
	account_0 rev0(id);
	account_1 rev1(id);
	account_2 rev2(id);
	account_3 rev3(id);
	account_4 rev4(id);
	account_5 rev5(id);


	if (!prev) {
		acct = new account(id);
	} else {
		acct = static_cast<account *>(prev);
	}

	switch(revision) {
	case 0:
		rev0.fromBlock(blk);
		break;
	case 1:
		rev1.fromBlock(blk);
		break;
	case 2:
		rev2.fromBlock(blk);
		break;
	case 3:
		rev3.fromBlock(blk);
		break;
	case 4:
		rev4.fromBlock(blk);
		break;
	case 5:
		rev5.fromBlock(blk);
		break;
	case 6:
		acct->fromBlock(blk);
		break;
	}

	switch(revision) {
	case 0:
		rev1.upgrade(rev0);
	case 1:
		rev2.upgrade(rev1);
	case 2:
		rev3.upgrade(rev2);
	case 3:
		rev4.upgrade(rev3);
	case 4:
		rev5.upgrade(rev4);
	case 5:
		acct->upgrade(rev5);
	case 6:
		break;
	default:
		delete acct;
		acct = NULL;
		break;
	}
	entry = acct;
	return entry;
}

esdbEntry *esdbAccountModule::decodeEntry(const QVector<genericField> &fields, bool doAliasMatch) const
{
	account *acct = new account(-1);
	QVector<QStringList> aliasedFields;

	QStringList acctNameAliases;
	acctNameAliases.push_back("name");
	if (doAliasMatch) {
		acctNameAliases.push_back("title");
		acctNameAliases.push_back("account");
		acctNameAliases.push_back("url");
		acctNameAliases.push_back("address");
	}

	QStringList usernameAliases;
	usernameAliases.push_back("username");
	if (doAliasMatch) {
		usernameAliases.push_back("user");
		usernameAliases.push_back("login");
		usernameAliases.push_back("email");
	}

	QStringList passwordAliases;
	passwordAliases.push_back("password");
	if (doAliasMatch) {
		passwordAliases.push_back("pass");
		passwordAliases.push_back("passphrase");
	}

	QStringList urlAliases;
	urlAliases.push_back("url");
	if (doAliasMatch) {
		urlAliases.push_back("address");
	}

	aliasedFields.push_back(acctNameAliases);
	aliasedFields.push_back(usernameAliases);
	aliasedFields.push_back(passwordAliases);
	aliasedFields.push_back(urlAliases);

	QStringList fieldNames;
	for (auto f : fields) {
		fieldNames.append(f.name);
	}

	QVector<QStringList::const_iterator> aliasMatched = aliasMatch(aliasedFields, fieldNames);
	QVector<QString> fieldValues = aliasMatchValues(aliasedFields, aliasMatched, fields, &acct->fields);
	acct->acctName = fieldValues[0];
	acct->userName = fieldValues[1];
	acct->password = fieldValues[2];
	acct->url = fieldValues[3];
	return acct;
}
