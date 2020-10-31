#include "BGSMusicTrack.h"
#include "Util/NameUtil.h"
#include "TESForm.h"

void GetCurrentMusic(ExtraInfoEntry* resultArray)
{
	RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();

	if (dataHandler)
	{
		logger::debug("Starting Music");

		RE::BSTArray<RE::TESForm*>* musicTrackArray = &(dataHandler->GetFormArray(RE::FormType::MusicTrack));

		RE::BSTArray<RE::TESForm*>::iterator itrEnd = musicTrackArray->end();

		//loop through all music tracks to check their status
		for (RE::BSTArray<RE::TESForm*>::iterator itr = musicTrackArray->begin(); itr != itrEnd; itr++)
		{
			RE::TESForm* musicForm = *itr;

			if (musicForm)
			{
				RE::BGSMusicTrackFormWrapper* musicFormWrapper = static_cast<RE::BGSMusicTrackFormWrapper*>(musicForm);
				RE::BSIMusicTrack::MUSIC_STATUS musicStatus = musicFormWrapper->GetMusicStatus();

				if (musicStatus == RE::BSIMusicTrack::MUSIC_STATUS::kPlaying)
				{
					std::string musicName = GetName(musicFormWrapper);
					ExtraInfoEntry* musicEntry;
					CreateExtraInfoEntry(musicEntry, "Music", musicName, priority_WorldData_Music);
					GetFormData(musicEntry, musicFormWrapper, nullptr);

					resultArray->PushBack(musicEntry);
				}

			}
		}
	}

}

/*

*/

/*RE::BSTArray<RE::TESForm*>* musicTypeArray = &(dataHandler->GetFormArray(RE::FormType::MusicType));

		RE::BSTArray<RE::TESForm*>::iterator itrEndType = musicTypeArray->end();

		std::vector<RE::BSIMusicTrack*> currentTracks;

		//loop through all music types to check which one is running and what the current track is for said type
		for (RE::BSTArray<RE::TESForm*>::iterator itr = musicTypeArray->begin(); itr != itrEndType; itr++)
		{
			RE::TESForm* baseForm = *itr;

			if (baseForm)
			{
				RE::BGSMusicType* musicType = static_cast<RE::BGSMusicType*>(baseForm);
				RE::BSIMusicType::MUSIC_STATUS musicStatus = musicType->typeStatus.get();

				if (musicStatus == RE::BSIMusicType::MUSIC_STATUS::kPlaying)
				{
					int currentTrackIndex = musicType->currentTrackIndex;

					if (musicType->tracks.size() > currentTrackIndex)
					{
						RE::BSIMusicTrack* currentTrack = musicType->tracks[currentTrackIndex];

						if (currentTrack)
						{
							currentTracks.push_back(currentTrack);
						}
					}
				}

			}
		}

		logger::debug("Checking Music tracks");
		//now loop through the actual tracks to find the track that matches the track we found in the previous step
		RE::BSTArray<RE::TESForm*>* musicTrackArray = &(dataHandler->GetFormArray(RE::FormType::MusicTrack));

		RE::BSTArray<RE::TESForm*>::iterator itrEndTrack = musicTrackArray->end();
		//loop through all music tracks to check their status
		for (RE::BSTArray<RE::TESForm*>::iterator itr = musicTrackArray->begin(); itr != itrEndTrack; itr++)
		{
			RE::TESForm* musicForm = *itr;

			if (musicForm)
			{
				RE::BGSMusicTrackFormWrapper* musicFormWrapper = static_cast<RE::BGSMusicTrackFormWrapper*>(musicForm);
				RE::BSIMusicTrack::MUSIC_STATUS musicStatus = musicFormWrapper->GetMusicStatus();

				if (musicStatus == RE::BSIMusicTrack::MUSIC_STATUS::kPlaying )
				{
					RE::BSIMusicTrack* msiMusicTrack = static_cast<RE::BSIMusicTrack*>(musicFormWrapper);

					if (std::count(currentTracks.begin(), currentTracks.end(), msiMusicTrack) > 0 )
					{
						std::string musicName = GetName(musicFormWrapper);
						ExtraInfoEntry* musicEntry;
						CreateExtraInfoEntry(musicEntry, "Music", musicName, priority_WorldData_Music);
						GetFormData(musicEntry, musicFormWrapper, nullptr);

						resultArray->PushBack(musicEntry);

					}
				}

			}
		}*/