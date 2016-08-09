<?php
require_once('../include/TbsCustomer.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* 
*/

abstract class CmdClientBillingAccountMediator
{

	function toTbsCustomer(ClientBillingAccount $clientBillingAccount, TbsCustomer $TbsCustomer){

		// mapped properties
			$TbsCustomer->Name = (string) $clientBillingAccount->getCompany(); 
			$TbsCustomer->AddressLine1 = (string) $clientBillingAccount->getAddress1();
			$TbsCustomer->AddressLine2 = (string) $clientBillingAccount->getAddress2();
			$TbsCustomer->City = (string) $clientBillingAccount->getCity();
			$TbsCustomer->State = (string) $clientBillingAccount->getState();
			$TbsCustomer->ZipCode = (string) $clientBillingAccount->getPostalcode();
			$TbsCustomer->Attention = (string) $clientBillingAccount->getBillingattention();
			$TbsCustomer->Email = (string) $clientBillingAccount->getBillingpocid();
			$TbsCustomer->Cycle = (string)$clientBillingAccount->getBillingcycle();
			$TbsCustomer->AgentName = (string) $clientBillingAccount->getCollectorid();
			//resolve country name
			$sql = "SELECT name, code FROM country WHERE id = " . pg_escape_literal($clientBillingAccount->getCountryid());
			$TbsCustomer->CountryName = sqlExecute($sql)->Fields('name');

			//name should be looked up
			$TbsCustomer->EnteredBy = 'CMD Automation'; //$clientBillingAccount->createdby

		return $TbsCustomer;
	}
}
?>