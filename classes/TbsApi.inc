<?php
$dir = dirname(__FILE__);
require_once('RestClient.inc');
require_once("log4php/Logger.php");

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* 
*post(string, $object)// $string is the url without the base portion, $object will be converted to json
*
* TODO: TBS date functions
* remaining looku[ functions
*/
class TbsApi extends RestClient
{
	private static $_instance;
	const TBSGUIURL = 'base url';
	var $base_url = 'bas url';
	var $content_type = 'application/json';
	var $apikey = '';
	var $api_status;
	var $api_result_length;

	//Filters for all data types
	protected $filtersAllowed = array(
		"Equal",
		"NotEqual",
		"GreaterThan",
		"GreaterThanOrEqual",
		"LessThan",
		"LessThanOrEqual",
		"IsNull",
		"In",
		"Notin"
	);

	// Filters for strings
	protected $stringFiltersAllowed = array(
		"Contains",
		"NotContains",
		"StartsWith",
		"NotStartsWith",
		"EndsWith",
		"NotEndsWith"
	);

	// conditional operators
	protected $conditionalOpsAllowed = array(
		"And",
		"Or",
		"Xor"
	);

	function __construct() 
	{
		parent::__construct($this->base_url, $this->content_type);
		curl_setopt($this->handler, CURLOPT_SSLVERSION, 6);
		Logger::configure('../config/log4php_TbsApi.xml');
		$this->Logger = Logger::getLogger(__CLASS__);
	}

	/*
	* singleton behaviour, with added TBS API headers
	*/
	public static function getInstance() {
	    if(is_null(self::$_instance)) {
			self::$_instance = new self();
			self::$_instance->base_url = (isDev() || isPreprod() || isUat()) ? 'base url here' : 'base url here';
			self::$_instance->apikey = ($GLOBALS['cmd_server'] == 'Dev') ? 'no pass' : 'no pass';
			self::$_instance->addHeader("User-Agent: CMD3");
			self::$_instance->addHeader("Authorization: Basic " . self::$_instance->apikey);
			self::$_instance->addHeader("Content-type: application/json");
			self::$_instance->addHeader("Accept: application/json");
			self::$_instance->addHeader("Host: demo3.precisebillonline.com");
		}
		return self::$_instance;
	}

	/*
	* Bases are like companies in TBS
	* return: array of ints, one for each base
	*/
    function getBasesList(){
    	$resourceUrl = "/bases?Take=All";
		$this->get($resourceUrl);
		// Check reponse status
		if($this->status_code != 200){
			$this->Logger->error(
				sprintf(
					'GET Call for Bases Failed.
					Status: %d
					API Error Message: 
					%s',
					$this->status_code,
					$this->response_body
				)
			);
			return false;
		} else {
			return $this->response_parsed;
		}
    }

	/*
	* $resource: string, the last url component wo preceeding /
	* $take: int | string, if string, "all", tells how many record to retrieve
	* $filters: array, see notes for format
	* $orders: array, see notes for format
	* return object | array
	* TODO: 
	*/
    function getResource($resource, $id = null, $take = null, $filters = null, $orders = null){
    	$resourceUrl = "/$resource";
    	// check if its a single resource, or collection call
    	if($id){
    		$resourceUrl .= "/$id";
    	} else {
    		if($take){
	    		$resourceUrl .= "?Take=$take";
    		}
    		// Check to see if filters are present
	    	if($filters){
    			$resourceUrl .= "&Filter=";
	    		foreach ($filters as $filter) {
	    			$resourceUrl .= ($filter['condition']) ? "." . $filter['condition'] . "." : "";
	    			$resourceUrl .= $filter['field'] ."." . $filter['filter'] . "(";
	    				if(is_array($filter['value'])){
			    			foreach ($filter['value'] as $key => $value) {
			    				//if its the first value, do not preface with a comma....
			    				$resourceUrl .= ($key == 0) ? urlencode($value) : ',' . urlencode($value) ;// maybe check array position instead?
			    			}
	    				} else {
	    					$resourceUrl .= urlencode($filter['value']);
	    				}

	    			$resourceUrl .= ")";	   		
	    		}
	    	}
	    	// check to see if ordering has been specified
	    	if($orders){
    			$resourceUrl .= "&Order=";
	    		foreach ($orders as $order) {
	    			$resourceUrl .= ($order['condition']) ? "." . $order['condition'] . "." : "";
	    			$resourceUrl .= $order['field'] . "(" . $order['value'] . ")";	    			
	    		}
	    	}
    	}

    	//Call external API
		$this->get($resourceUrl);

		// check the request status
		if($this->status_code == 0){
			$this->Logger->error(
				sprintf(
					'GET Call for resource \'%s\' Failed.
					Status: %d,
					Request: %s,
					API Error Message: 
					%s',
					$resourceUrl,
					$this->status_code,
					$this->request_body_raw,
					'The Service Did Not Respond'
				)
			);
			throw new Exception('The Service Failed to Respond.');
		 } elseif ($this->status_code != 200){
			$this->Logger->error(
				sprintf(
					'GET Call for resource \'%s\' Failed.
					Status: %d,
					Request: %s,
					API Error Message: 
					%s',
					$resourceUrl,
					$this->status_code,
					$this->request_body_raw,
					$this->response_body
				)
			);
			throw new Exception($this->response_body);
		} else {
			return $this->response_parsed;
		}
    }

	/*
	* This is a adapter for the parent class function.
	* it adds a / to the resource name for consistency
	* $reource : string 'resourcename'
	* $data : object | ?array?
	*/
	function post($resource, $data = null){
		// make the API call
		parent::post('/' . $resource, $data);

				// check the request status
		if($this->status_code != 200){
			$this->Logger->error(
				sprintf(
					'POST Call for resource \'%s\' Failed.
					Status: %d, 
					Request: %s,
					API Error Message: 
					%s',
					$resource,
					$this->status_code,
					$this->request_body_raw,
					$this->response_body
				)
			);
			throw new Exception($this->response_body);
		} else {
			return $this->response_parsed;
		}
	}

	/*
	 * implemented for the RestClient class
	 * todo: update the parent class with this method, then create a arapper for this class ex: post()
	 */
	function put($resource, $data = null) {
		
		if(isset($data)) {
			$this->request_body = $data;
		}

		// make the call chief
		$this->exec ( "PUT", '/' . $resource );

		// check the request status
		if($this->status_code != 200){
			$this->Logger->error(
				sprintf(
					'GET Call for resource \'%s\' Failed.
					Status: %d,
					Request: %s
					API Error Message: 
					%s',
					$resource,
					$this->status_code,
					$this->request_body_raw,
					$this->response_body
				)
			);
			throw new Exception($this->response_body);
		} else {
			return $this->response_parsed;
		}
	} 

	/*
	*get the plan type for a given key
	* $planTypeKey : int
	* return: string, the name of the plan type
	*/
	function lookupPlanType($planTypeKey){
		$resource = "plantypes";
		$planTypesArray = $this->getResource($resource);
		if($planTypesArray){
			foreach ($planTypesArray as $planType) {
				if($planType->Key == $planTypeKey){
					return $planType->Value;
				}
			}
		}
		return false;
	}

	/*
	*get the simple plan for a given key
	* $planTypeKey : int
	* $planKey : int
	* return: string, the name of the plan type
	*/
	function lookupSimplePlan($planTypeKey, $planKey){
		$resource = "plantypes/$planTypeKey/simpleplans";
		$simplePlansArray = $this->getResource($resource);
		if($simplePlansArray){
			foreach ($simplePlansArray as $plan) {
				if($plan->Key == $planKey){
					return $plan->Value;
				}
			}
		}
		return false;
	}

	/*
	*get the invoice category for the given key
	* $categoryKey : int
	* return: string, the description of the invoice category
	*/
	function lookupInvoiceCategory($categoryKey){
		$resource = "lkinvoicecategories/$categoryKey";
		$invoiceCategory = $this->getResource($resource);
		if($invoiceCategory){
			return $invoiceCategory->Description;
		}
		return false;
	}

	/*
	*get the invoice category for the given key
	* $transClassKey : int
	* return: string, the description of the trans class category
	*/
	function lookupTransClass($transClassKey){
		$resource = "lktransclassifications/$transClassKey";
		$transClassCategory = $this->getResource($resource);
		if($transClassCategory){
			return $transClassCategory->Description;
		}
		return false;
	}

	/*
	*get the tax transaction type for a given key
	* $taxTransTypeKey : int
	* return: string, associated text
	*/
	function lookupTaxTransType($taxTransTypeKey){
		$resource = "tax/transtypes";
		$taxTransTypeArray = $this->getResource($resource);
		if($taxTransTypeArray){
			foreach ($taxTransTypeArray as $taxTransType) {
				if($taxTransType->m_Item1 == $taxTransTypeKey){
					return $taxTransType->m_Item2;
				}
			}
		} 
		return false;
	}

	/*
	*get the tax service type for a given key
	* $taxTransTypeKey : int
	* $taxServTypeKey : int
	* return: string, associated text
	*/
	function lookupTaxServType($taxTransTypeKey, $taxServTypeKey){
		$resource = "tax/transtypes/$taxTransTypeKey/servtypes";
		$taxServTypeArray = $this->getResource($resource);
		if($taxServTypeArray){
			foreach ($taxServTypeArray as $taxServType) {
				if($taxServType->m_Item1 == $taxServTypeKey){
					return $taxServType->m_Item2;
				}
			}
		}
		return false;
	}

	/*
	* converts a human readable date into an ASP format one
	* params: 
	* $humanDate: string, ex: 2013-08-07 or 2013-02-14 13:55:27 or special cases like TODAY, NOW
	*
	* returns : string on success, false upon failure
	*/
	function getAspDate($humanDate){
		return $this->post('utilities/getdatetime/asp', $humanDate);
	}

	/*
	* converts an ASP format date human readable date 
	* $aspDate: string, ex: 1375329600000-0400
	*
	* returns : string on success, false upon failure
	*/
	function getHumanDate($aspDate){
		return $this->post('utilities/getdatetime/human', $aspDate);
	}

	/*
	* EX: /Date(928164000000-0400)/ The API will convert this. format = Date(unix timestamp-time zone)
	* $timestamp : int, unix timestamp
	* $timezone : int, unknown format
	* return : string, tbs formatted date
	*/
    function convertTimestamp($timestamp, $timezone = "-0000"){
    	return "/Date($timestamp$timezone)/";
    }

	/*
	* EX: /Date(928164000000-0400)/ The API will convert this. format = Date(unix timestamp-time zone)
	* $tbsDate : string, unix timestamp
	* return : string, tbs formatted date
	*/
    function convertTbsDate($tbsDate){
    	//regex to extract timestamp
		preg_match(
			'/(?:Date\()([0-9]+)(?:\+|\-)/',
		    $tbsDate, 
		    $matches
	    );
	    // grab the capture group 
	    $epochTimeInMilliseconds = $matches[1];
    	// regex to extract time zone: not needed currently
	    //convert milliseconds to seconds
		$timestamp = round($epochTimeInMilliseconds / 1000);
    	try {
		    $dateTime = new DateTime('@' . $timestamp);

		} catch (Exception $e) {
			$this->Logger->error(
				sprintf(
					'Conversion of a TBS date failed.
					TBS date: %s
					Exception: %s',
					$tbsDate,
					$e->getMessage()
				)
			);
		    exit('An error has occured converting a TBS date. Please see the TBS API log file for more details.');
		}
		return $dateTime->format('Y-m-d');
    }
    /* temp to test for replacement
    * the problem with the parent class function was that it converts objects to arrays, and arrays to array. basically everything is an array. This is a problem for the TBS system, as it expects us to GET objects, and nest them as objects into other objects. TBS will not take an array!
	 * @private
	 */
	function parse() {
		/* JSON */
		if (strpos ( $this->response_content_type, "application/json" ) !== false) {
			$this->response_parsed = json_decode($this->response_body);
			/* JSON STREAM */
		} elseif (strpos ( $this->response_content_type, "json-stream" ) !== false) {
			$stream = split ( "\n", $this->response_body );
			if (count ( $stream ) < $stream [0] ['ResultLength']) {
				// echo "Invalid JSON Stream. Result Length:".count($stream);
				$this->client->status_code = 400;
				$GLOBALS ['ResultStack']->pushNew ( "Invalid JSON Stream", ERROR );
				return false;
			}
			$jsonServer = new JSON ( JSON_LOOSE_TYPE );
			$this->response_parsed = array ();
			foreach ( $stream as $line )
				$this->response_parsed [] = $jsonServer->decode ( $line );
			/* DEFAULT */
		} else {
			$this->response_parsed = $this->response_body;
		}
	}

	/*
	 * initialize request-specific configuration
	 */
	function prepare($method, $path) {
		curl_setopt ( $this->handler, CURLOPT_HTTPHEADER, $this->request_headers );
		curl_setopt ( $this->handler, CURLOPT_URL, $this->base_url . $path );
		curl_setopt ( $this->handler, CURLOPT_CUSTOMREQUEST, $method );
		
		if ($method == 'POST' || $method == 'PUT') {
			if ($this->content_type == 'application/json') {
				$jsonServer = new JSON ( JSON_LOOSE_TYPE );
				$this->request_body_raw = json_encode( $this->request_body );
			} else {
				$this->request_body_raw = $this->request_body;
			}
			
			curl_setopt ( $this->handler, CURLOPT_POSTFIELDS, $this->request_body_raw );
		}

	}

	/*
	* params:
	* $iso2code : the two digit iso country designation to be looked up
	* return: TBS numeric country key, false upon failure
	*/
	function getNumericCountryCode($iso2Code){
		$resource = "countries";
		$countriesArray = $this->getResource($resource, null, 'All');
		if($countriesArray){
			foreach ($countriesArray as $country) {
				if($country->Alpha2Code == $iso2Code){
					return $country->NumericCode;
				}
			}
		}
		return false;
	}

	/*
	* validates a postal code with tbs api
	* params: $postal code :  string , the postal code to validate
	*
	*/
	function validateCountryPostalCode($postalCode, $tbsCountriesCode){
		// build filter
		$filters = array(
			array(
					"field" => 'NumericCode',
					"filter" => 'Equal',
					"value" => $tbsCountriesCode
				)
		);
		// call resource
		$tbsResult = $this->getResource('countries', null, 'All', $filters);
		// appply the regex 
		if(preg_match('/' . $tbsResult[0]->ZipcodeRegex . '/', $postalCode) === 1 || $tbsResult[0]->ZipcodeRegex == null) {
			return true;
		} else {
			return false;
		}
	}

	/*
	* Author: Reiser Mayne
	* get the tbs customer key from the tbs account number
	* params: tbs acccount number
	*
	*
	*/
	function getCustomersKeyFromAccountNumber($tbsAccountNumber){
		// build filter
		$filters = array(
			array(
					"field" => 'AccountNumber',
					"filter" => 'Equal',
					"value" => $tbsAccountNumber
				)
		);
		// call resource
		$tbsResult = $this->getResource('customers', null, 'All', $filters);

		if($tbsResult[0]->CustomersKey) {
			return $tbsResult[0]->CustomersKey;
		} else {
			return false;
		}
	}


	/*
	* Author: Reiser Mayne
	* Date 4/8/2016
	* Description: looks up the passed zipcode in order to determine if its taxable in that city
	* return : true if in the city, false if ourside the city or if its unknown
	*/
	function isZipInCity($zip){
		$filters = array(
			array(
					"field" => 'Zip',
					"filter" => 'Equal',
					"value" => $zip
				)
		);
		$tbsResult = $this->getResource('lkzipcodes', null, 'All', $filters);
		$count = count($tbsResult);
			if($count > 1){
				foreach ($tbsResult as $zipCodeRecord) {
					if($zipCodeRecord->DEFAULT == "Y") {
						$inCity = $zipCodeRecord->INOUTCITY;
					}	
				}
			} elseif ($count == 1){
				$inCity = $tbsResult[0]->INOUTCITY;
			} else {
				return false;
			}

		switch($inCity){
			case "I":
			// in city
			return true;

			case "O":
			// out of city
			return false;

			case "B":
			// both
			return true;

			case null:
			// not set
			return false;

			default :
			return false;
		}
	}
}
