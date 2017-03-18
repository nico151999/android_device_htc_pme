/*
 * Copyright (C) 2015 crDroid Android
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.android.settings.beanstalk;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceGroup;
import android.support.v7.preference.PreferenceScreen;

import com.android.settings.SettingsPreferenceFragment;
import com.android.internal.logging.MetricsProto.MetricsEvent;
import com.android.settings.R;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class About extends SettingsPreferenceFragment {

    public static final String TAG = "About";

    private static final String KEY_BEANSTALK_SHARE = "share";
    private static final String KEY_BEANSTALK_SLACK = "slack";

    Preference mDownloadsUrl;
    Preference mSourceUrl;
    Preference mFacebookUrl;
    Preference mDonationUrl;
    Preference mMaintainerDonationUrl;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.about_beanstalk);

        mDownloadsUrl = findPreference("beanstalk_downloads");
        mSourceUrl = findPreference("beanstalk_source");
        mFacebookUrl = findPreference("beanstalk_facebook");
	mDonationUrl = findPreference("beanstalk_donation");
        mMaintainerDonationUrl = findPreference("developer");
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.BEANSTALK;
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        if (preference == mSourceUrl) {
            launchUrl("https://github.com/scotthartbti");
        } else if (preference == mFacebookUrl) {
            launchUrl("http://www.facebook.com/pages/Beanstalk/421499131276932");
        } else if (preference == mDonationUrl) {
            launchUrl("http://forum.xda-developers.com/donatetome.php?u=4552996");
        } else if (preference == mMaintainerDonationUrl) {
            launchUrl("http://forum.xda-developers.com/donatetome.php?u=5880204");
        } else if (preference == mDownloadsUrl) {
            launchUrl("https://androidfilehost.com/?w=devices&uid=23159073880933293");
        } else if (preference.getKey().equals(KEY_BEANSTALK_SHARE)) {
        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_SEND);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_TEXT, String.format(
                getActivity().getString(R.string.share_message), Build.MODEL));
        startActivity(Intent.createChooser(intent, getActivity().getString(R.string.share_chooser_title)));
        } else if (preference.getKey().equals(KEY_BEANSTALK_SLACK)) {
	Intent email = new Intent(Intent.ACTION_SEND, Uri.parse("mailto:"));
        email.setType("message/rfc822");
	email.putExtra(Intent.EXTRA_EMAIL,new String[] { "beanstalk.fto5a@zapiermail.com" });
	email.putExtra(Intent.EXTRA_SUBJECT, String.format(
                getActivity().getString(R.string.slack_subject), Build.MODEL));
        email.putExtra(Intent.EXTRA_TEXT, String.format(
                getActivity().getString(R.string.join_slack), Build.MODEL));
        startActivity(Intent.createChooser(email, getActivity().getString(R.string.join_slack_title)));
        }
        return super.onPreferenceTreeClick(preference);
    }

    private void launchUrl(String url) {
        Uri uriUrl = Uri.parse(url);
        Intent donate = new Intent(Intent.ACTION_VIEW, uriUrl);
        getActivity().startActivity(donate);
    }
}
