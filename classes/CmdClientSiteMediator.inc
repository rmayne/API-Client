<?php
require_once('../include/TbsLocation.inc');
require_once('../include/TbsApi.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* TODO: 
*/

abstract class CmdClientSiteMediator
{

	function toTbsLocation(ClientSite $clientSite, TbsLocation $tbsLocation){

		$tbsApi = TbsApi::getInstance();

		$sql = "SELECT name FROM client WHERE id = "  . pg_escape_literal($clientSite->getClientid()) . ";";
		$clientName = sqlGetOne($sql);

		$sql = "SELECT name, code FROM country WHERE id = " . pg_escape_literal($clientSite->getCountryid());
		$countryName = sqlExecute($sql)->Fields('name');
		$cmdCountryCode = sqlExecute($sql)->Fields('code');
		$tbsCountryCode = $tbsApi->getNumericCountryCode($cmdCountryCode);

		$tbsLocation->Name = $clientName;
	    $tbsLocation->Description = $clientSite->getAddress1();
		$tbsLocation->AddressLine1 = $clientSite->getAddress1();
		$tbsLocation->AddressLine2 = $clientSite->getAddress2();
		$tbsLocation->City = $clientSite->getCity(); 
		$tbsLocation->State = $clientSite->getState();
	    $tbsLocation->CountryCode = $tbsCountryCode;
		$tbsLocation->ZipCode = $clientSite->getPostalcode(); // this is to pass TBS validation
		$tbsLocation->CountryName = $countryName;
		$tbsLocation->ContactEmail = $clientSite->getSiteemail();
		$tbsLocation->MainPhone = $clientSite->getSitephone();
		$tbsLocation->ContactName = $clientSite->getSitecontact();

		return $tbsLocation;
	}
}
